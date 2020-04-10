#include "Server.hpp"
#include "Connection.hpp"
#include "Descriptor.hpp"
#include "Errors.hpp"
#include "utils.hpp"
#include <cstring>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>



namespace tcp {
    static const size_t MAX_EVENTS = 10000;

    Server::EpollManager::EpollManager(Server &server) :
            server_(server), epollObject_(epoll_create(1)) {

        if (epollObject_ == -1) {
            throw epoll_error(std::string("Epoll creation error: ")
                              + std::strerror(errno));
        }

        epoll_event Event{};
        createEvent(&Event, &server_.masterSocket_, EPOLLIN);
        addEvent(server.masterSocket_, Event);
    }

    void Server::EpollManager::run() {
        while (true) {
            static epoll_event Events[MAX_EVENTS];

            int nfds = ::epoll_wait(epollObject_, Events, MAX_EVENTS, -1);
            if (nfds < 0) {
                if (errno == EINTR) continue;
                throw epoll_error(std::string("epoll_wait error: ") + std::strerror(errno));
            }

            for (int i = 0; i < nfds; ++i) {
                auto ptr = Events[i].data.ptr;

                if (ptr == &server_.masterSocket_) {
                    acceptClients();
                } else {
                    Connection &connection = *(static_cast<Connection *>(ptr));
                    uint32_t event = Events[i].events;
                    handleClient(connection, event);
                }
            }
        }
    }

    void Server::EpollManager::acceptClients() {
        size_t i = 0;

        while (i < MAX_EVENTS) {
            Connection con;
            sockaddr_in client_addr{};
            socklen_t len = sizeof(client_addr);
            Descriptor::Descriptor connect_fd(::accept(server_.masterSocket_,
                    reinterpret_cast<sockaddr *>(&client_addr), &len));

            if (connect_fd == -1) {
                if (errno == EINTR) continue;
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                    return;
                else
                    throw accept_error(std::string("Accept failed: ") + std::strerror(errno));
            }

            if (set_nonblock(connect_fd) == -1) {
                continue;
            }

            get_binded_ip_port(connect_fd.data(), con.src_addr_, con.src_port_);
            con.dst_addr_ = ::inet_ntoa(client_addr.sin_addr);
            con.dst_port_ = ntohs(client_addr.sin_port);
            con.fd_ = std::move(connect_fd);

            addNewConnection(std::move(con));
        }
    }

    void Server::EpollManager::handleClient(Connection &connection, uint32_t &event) {
        char buf;

        if (event & EPOLLERR || event & EPOLLHUP) {
            deleteConnection(connection);
        } else {
            uint16_t old_event = event;
            try {
                server_.callback_(connection, event);
            } catch (...) {
                deleteConnection(connection);
            }
            if (!connection.is_opened()) {
                deleteConnection(connection);
            } else if (old_event != event) {
                changeEvent(connection, event);
            }
        }
    }

    void Server::EpollManager::addNewConnection(Connection &&connection) {
        int fd = connection.fd_;
        slaveSockets_[connection.fd_] = std::move(connection);

        epoll_event Event {};
        createEvent(&Event, &slaveSockets_[fd], EPOLLIN);
        addEvent(fd, Event);
    }

    void Server::EpollManager::deleteConnection(Connection &connection) {
        slaveSockets_.erase(connection.fd_);
    }

    void Server::EpollManager::addEvent(int fd, epoll_event &Event) {
        if (::epoll_ctl(epollObject_, EPOLL_CTL_ADD, fd, &Event) == -1) {
            throw epollAddError(std::string("Epoll add error: ")
                                + std::strerror(errno));
        }
    }

    void Server::EpollManager::changeEvent(Connection &connection, uint32_t new_event) {
        epoll_event Event{};
        createEvent(&Event, &connection, new_event);

        if (::epoll_ctl(epollObject_, EPOLL_CTL_MOD, connection.fd_, &Event) == -1) {
            throw epoll_error(std::string("Epoll mod error: ")
                                + std::strerror(errno));
        }
    }
}