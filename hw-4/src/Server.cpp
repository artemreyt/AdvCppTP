#include "Server.hpp"
#include "Errors.hpp"
#include "Descriptor.hpp"
#include "utils.hpp"
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <unordered_map>

namespace tcp {

    static const int g_default_max_connections = SOMAXCONN;
    static const size_t MAX_EVENTS = 1000;

    Server::Server(const std::string &ip, uint16_t port, Callback_t callback):
                    callback_(std::move(callback)) {
        open(ip, port);
    }

    Server::~Server() noexcept {
        close();
    }

    bool Server::is_opened() const {
        return masterSocket_.is_valid();
    }


    void Server::open(const std::string &ip, uint16_t port) {
        Process::Descriptor fd(::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP));
        if (fd.data() == -1) {
            throw socket_error(std::string("Socket creation error: ") + std::strerror(errno));
        }

        int optval = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        if (!inet_aton(ip.c_str(), &addr.sin_addr)) {
            throw bad_ip_address(ip);
        }
        addr.sin_port = htons(port);

        if (::bind(fd.data(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
            throw socket_error(std::string("Bind error: ") + std::strerror(errno));
        }

        if (::listen(fd.data(), g_default_max_connections) == -1) {
            throw socket_error("Listen error with max_connections = " +
                               std::to_string(g_default_max_connections));
        }

        masterSocket_ = std::move(fd);
    }

    Process::Descriptor Server::createEpoll() {
        Process::Descriptor Epoll(::epoll_create(1));

        if (Epoll == -1) {
            close();
            throw epoll_error(std::string("Epoll creation error: ")
                              + std::strerror(errno));
        }

        epoll_event Event {};
        createEvent(&Event, &masterSocket_, EPOLLIN);
        addEvent(Epoll, masterSocket_, &Event);

        return std::move(Epoll);
    }

    void Server::addEvent(int epollObject, int fd, epoll_event *Event) {
        if (::epoll_ctl(epollObject, EPOLL_CTL_ADD, fd, Event) == -1) {
            throw epollAddError(std::string("Epoll add error: ")
                                + std::strerror(errno));
        }
    }

    void Server::eventLoop() {
        auto epollObject = createEpoll();
        std::unordered_map<int, Connection> SlaveSockets;

        while ( true ) {
            static epoll_event Events[MAX_EVENTS];

            int nfds = ::epoll_wait(epollObject, Events, MAX_EVENTS, -1);
            if (nfds < 0) {
                if (errno == EINTR) continue;
                throw epoll_error(std::string("epoll_wait error: ") + std::strerror(errno));
            }

            for (int i = 0; i < nfds; ++i) {
                auto ptr = Events[i].data.ptr;

                if (ptr == &masterSocket_) {
                    acceptClients(epollObject, SlaveSockets);
                } else {
                    Connection &connection = *(static_cast<Connection *>(ptr));
                    auto event = Events[i].events;
                    handleClient(epollObject, connection, event, SlaveSockets);
                }
            }
        }
    }

    void Server::acceptClients(int epollObject, std::unordered_map<int, Connection> &slaveSockets) {
        int i = 0;
        while (i++ < MAX_EVENTS) {
            Connection con;
            sockaddr_in client_addr{};
            socklen_t len = sizeof(client_addr);
            Process::Descriptor connect_fd(::accept(masterSocket_,
                    reinterpret_cast<sockaddr *>(&client_addr), &len));

            if (connect_fd == -1) {
                if (errno == EINTR) continue;
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                    return;
                else
                    throw accept_error(std::string("Accept failed: ") + std::strerror(errno));
            }
            get_binded_ip_port(connect_fd.data(), con.src_addr_, con.src_port_);
            con.dst_addr_ = ::inet_ntoa(client_addr.sin_addr);
            con.dst_port_ = ntohs(client_addr.sin_port);
            con.fd_ = std::move(connect_fd);

            addNewConnection(epollObject, std::move(con), slaveSockets);
        }
    }


    void Server::addNewConnection(int epollObject, Connection &&connection,
            std::unordered_map<int, Connection>& slaveSockets) {
        int fd = connection.fd_;
        slaveSockets[connection.fd_] = std::move(connection);

        epoll_event Event {};
        createEvent(&Event, &slaveSockets[fd], EPOLLIN);
        addEvent(epollObject, fd, &Event);
    }

    void Server::set_max_connect(int max_connect) {
        if (is_opened()) {
            ::listen(masterSocket_, max_connect);
        }
    }

    void Server::handleClient(int epollObject, Connection &connection,
                      uint32_t event, std::unordered_map<int, Connection> &slaveSockets) {
        char buf;

        if (event & EPOLLERR || event & EPOLLHUP || (
           (event & EPOLLIN) && (::recv(connection.fd_, &buf, 1, MSG_PEEK) == 0))
           ){
            deleteConnection(connection, slaveSockets);
        } else {
            try {
                callback_(connection, static_cast<event_t>(event));
            } catch (...) {
                deleteConnection(connection, slaveSockets);
            }
        }
    }

    void Server::deleteConnection(Connection &connection,
                          std::unordered_map<int, Connection> &slaveSockets) {
        connection.fd_.close();
        slaveSockets.erase(connection.fd_);
    }

    void Server::changeCallback(Callback_t callback) {
        callback_ = std::move(callback);
    }

    void Server::close() {
        if (is_opened()) {
            ::shutdown(masterSocket_, SHUT_RDWR);
            masterSocket_.close();
        }
    }
}
