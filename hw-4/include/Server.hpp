//
// Created by Artem Starshov on 2020-04-01.
//

#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "Descriptor.hpp"
#include "Connection.hpp"
#include <string>
#include <cstdint>
#include <memory>
#include <set>
#include <functional>
#include <sys/epoll.h>

namespace tcp {

    typedef std::function<void(Connection&, uint32_t)> Callback_t;

    class Server {
    public:
        Server(const std::string &ip, uint16_t port, Callback_t callback);
        ~Server() noexcept;

        void eventLoop();
        void changeCallback(Callback_t callback);
        bool is_opened() const;
        void open(const std::string &ip, uint16_t port);
        void set_max_connect(int max_connect);
        void close();

    protected:
        Process::Descriptor createEpoll();
        void addNewConnection(int epollObject, Connection &&connection, std::set<Connection>& slaveSockets);
        void addEvent(int epollObject, epoll_event *Event);
        void acceptClients(int epollObject, std::set<Connection> &slaveSockets);
        void handleClient(int epollObject, Connection &connection,
                        uint32_t event, std::set<Connection> &SlaveSockets);

        Process::Descriptor masterSocket_;
        Callback_t callback_;
    };
}

#endif // TCP_SERVER_HPP
