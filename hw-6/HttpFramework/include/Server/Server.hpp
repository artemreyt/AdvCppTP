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
#include <unordered_map>
#include <functional>
#include <sys/epoll.h>
#include <thread>
#include <chrono>

namespace HttpFramework {
    namespace Server {

        using namespace std::chrono;
        class EpollManager;


        class Server {
        public:
            Server(const std::string &ip, uint16_t port, Logger::BaseLogger &logger,
                   std::chrono::seconds read = 2s, std::chrono::seconds write = 2s);

            ~Server() noexcept;

            void run(size_t number_threads = std::thread::hardware_concurrency());

            virtual http_structures::HttpResponse onRequest(const http_structures::HttpRequest &request) = 0;

            bool is_opened() const;

            void open(const std::string &ip, uint16_t port);

            void set_max_connect(int max_connect);

            void close();

        protected:

            friend class EpollManager;

            Descriptor::Descriptor masterSocket_;
            Logger::BaseLogger &logger_;
            const std::chrono::seconds read_timeout_ = 2s;
            const std::chrono::seconds write_timeout_ = 2s;
        };
    }

}

#endif // HTTP_SERVER_HPP
