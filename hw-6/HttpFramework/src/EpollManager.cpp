#include "Server.hpp"
#include "Connection.hpp"
#include "Descriptor.hpp"
#include "Coroutine.hpp"
#include "Errors.hpp"
#include "HttpRequest.hpp"
#include "utils.hpp"
#include <cstring>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <functional>
#include <tuple>
#include <stdexcept>

namespace HttpFramework {
    static const size_t MAX_EVENTS = 1000;
    static const size_t MAX_ACCEPTIONS = 1;
    static const uint32_t MASTER_SOCKET_EVENTS = EPOLLIN | EPOLLEXCLUSIVE;

    Server::EpollManager::EpollManager(Server &server) :
            server_(server), epollObject_(epoll_create(1)) {

        if (epollObject_ == -1) {
            throw epoll_error(std::string("Epoll creation error: ")
                              + std::strerror(errno));
        }

        epoll_event Event{};
        createEvent(&Event, &server_.masterSocket_, MASTER_SOCKET_EVENTS);
        addEvent(server.masterSocket_, Event);
    }

    void Server::EpollManager::operator()() {
        run();
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
                    handleClient(Events[i]);
                }
            }
        }
    }

    void Server::EpollManager::acceptClients() {
        size_t i = 0;

        while (i < MAX_ACCEPTIONS) {
            Connection con;
            sockaddr_in client_addr{};
            socklen_t len = sizeof(client_addr);
            Descriptor::Descriptor connect_fd(::accept(server_.masterSocket_,
                    reinterpret_cast<sockaddr *>(&client_addr), &len));

            if (connect_fd == -1) {
                if (errno == EINTR) continue;
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                    return;
                throw accept_error();
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

    void Server::EpollManager::handleClient(const epoll_event &event) {
        auto id = event.data.u64;

        if (event.events & EPOLLERR || event.events & EPOLLHUP) {
            deleteConnection(id);
            return;
        }

        try {
            Coroutine::resume(id);
        } catch (...) {
            // TODO: logging
            deleteConnection(id);
        }
    }

    void Server::EpollManager::addNewConnection(Connection &&connection) {
        int id = connection.fd_;

        connectionsMap.emplace(connection.fd_, std::move(connection));
        Coroutine::create(
                id,
                &Server::EpollManager::clientRoutine,
                std::ref(*this)
                );

        epoll_event Event {};
        Event.data.u32 = id;
        Event.events = EPOLLIN;

        addEvent(id, Event);
    }

    void Server::EpollManager::deleteConnection(Coroutine::routine_t id) {
        connectionsMap.erase(id);
        Coroutine::finish(id);
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

    void Server::EpollManager::clientRoutine() {
        auto &connection = connectionsMap.at(Coroutine::current());

        while (true) {
            HttpRequest request(connection);
            request.receive_request();

            HttpResponse response = server_.onRequest(request);
            changeEvent(connection, EPOLLOUT);
            response.send(connection);

            if (request.get_headers().at("Connection") != "Keep-Alive") {
                changeEvent(connection, EPOLLIN);
                break;
            }
        }
        connectionsMap.erase(connection.fd_);
    }
}