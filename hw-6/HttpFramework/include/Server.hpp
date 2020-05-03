#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "Descriptor.hpp"
#include "Connection.hpp"
#include "Coroutine.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <functional>
#include <sys/epoll.h>
#include <thread>

namespace HttpFramework {

    class Server {
    public:
        Server(const std::string &ip, uint16_t port);
        ~Server() noexcept;

        void run(size_t number_threads=std::thread::hardware_concurrency());
        virtual HttpResponse onRequest(const HttpRequest &request) = 0;
        bool is_opened() const;
        void open(const std::string &ip, uint16_t port);
        void set_max_connect(int max_connect);
        void close();

    protected:
        Descriptor::Descriptor masterSocket_;

        class EpollManager : public std::allocator_arg_t {
        public:
            explicit EpollManager(Server &server);
            void run();
            void operator()();

        private:
            void addNewConnection(Connection &&connection);
            void deleteConnection(Connection &connection);
            void addEvent(int fd, epoll_event &Event);
            void changeEvent(Connection &connection, uint32_t new_event);
            void acceptClients();
            void handleClient(Connection &connection, uint32_t &event);
            void clientRoutine(Connection &connection);

            Descriptor::Descriptor epollObject_;
            Server &server_;
            std::unordered_map<Coroutine::routine_t, Connection> slaveSockets_;
        };
    };
}

#endif // HTTP_SERVER_HPP
