#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "Descriptor.hpp"
#include "Connection.hpp"
#include "Coroutine.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include <functional>
#include <sys/epoll.h>
#include <thread>

namespace HttpFramework {

    class Server {
    public:
        Server(const std::string &ip, uint16_t port, Logger::BaseLogger &logger);
        ~Server() noexcept;

        void run(size_t number_threads=std::thread::hardware_concurrency());
        virtual HttpResponse onRequest(const HttpRequest &request) = 0;
        bool is_opened() const;
        void open(const std::string &ip, uint16_t port);
        void set_max_connect(int max_connect);
        void close();

    protected:
        Descriptor::Descriptor masterSocket_;
        Logger::BaseLogger &logger_;


    public:
        class EpollManager {
        public:
            explicit EpollManager(Server &server);
            EpollManager(EpollManager &&other) = default;
            EpollManager(const EpollManager &other) = delete;
            void run();
            void operator()();

        private:
            void addNewConnection(Connection &&connection);
            void deleteConnection(Coroutine::routine_t id);
            void addEvent(int fd, epoll_event &Event);
            void changeEvent(Connection &connection, uint32_t new_event);
            void acceptClients();
            void handleClient(const epoll_event &event);
            void clientRoutine();

            Descriptor::Descriptor epollObject_;
            std::map<int, Connection> connectionsMap;
            Server &server_;
        };
    };
}

#endif // HTTP_SERVER_HPP
