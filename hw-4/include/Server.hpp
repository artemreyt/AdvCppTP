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
#include <unordered_map>
#include <functional>
#include <sys/epoll.h>

namespace tcp {
    constexpr uint32_t  READ_EVENT = EPOLLIN;
    constexpr uint32_t  WRITE_EVENT = EPOLLOUT;

    typedef std::function<void(Connection&, uint32_t&)> Callback;

    class Server {
    public:
        Server(const std::string &ip, uint16_t port, const Callback &callback);
        ~Server() noexcept;

        void eventLoop();
        void changeCallback(Callback callback);
        bool is_opened() const;
        void open(const std::string &ip, uint16_t port);
        void set_max_connect(int max_connect);
        void close();

    protected:
        Descriptor::Descriptor masterSocket_;
        Callback callback_;

        class EpollManager {
        public:
            explicit EpollManager(Server &server);
            void run();

        private:
            void addNewConnection(Connection &&connection);
            void deleteConnection(Connection &connection);
            void addEvent(int fd, epoll_event &Event);
            void changeEvent(Connection &connection, uint32_t new_event);
            void acceptClients();
            void handleClient(Connection &connection, uint32_t &event);

            Descriptor::Descriptor epollObject_;
            Server &server_;
            std::unordered_map<int, Connection> slaveSockets_;
        };
    };
}

#endif // TCP_SERVER_HPP
