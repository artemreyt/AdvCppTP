#include "Server/Server.hpp"
#include "Errors.hpp"
#include "Descriptor.hpp"
#include "Logger.hpp"
#include "EpollManager.hpp"
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

namespace HttpFramework::Server {

    static const int    g_default_max_connections = SOMAXCONN;
    static const size_t g_max_threads = 16;

    Server::Server(const std::string &ip, uint16_t port, Logger::BaseLogger &logger,
           std::chrono::seconds read, std::chrono::seconds write):
        logger_(logger), read_timeout_(read), write_timeout_(write) {
        open(ip, port);
    }

    Server::~Server() noexcept {
        close();
    }

    bool Server::is_opened() const {
        return masterSocket_.is_valid();
    }

    void Server::open(const std::string &ip, uint16_t port) {
        Descriptor::Descriptor fd(::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP));
        if (fd.data() == -1) {
            throw socket_creation_error(std::string("Socket creation error: ") + std::strerror(errno));
        }

        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        if (!inet_aton(ip.c_str(), &addr.sin_addr)) {
            throw bad_ip_address(ip);
        }
        addr.sin_port = htons(port);

        int optval = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        if (::bind(fd.data(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
            throw tcp_error(std::string("Bind error: ") + std::strerror(errno));
        }

        if (::listen(fd.data(), g_default_max_connections) == -1) {
            throw tcp_error("Listen error with max_connections = " +
                      std::to_string(g_default_max_connections));
        }

        masterSocket_ = std::move(fd);
    }

    void Server::set_max_connect(int max_connect) {
        if (::listen(masterSocket_, max_connect) == -1) {
            throw tcp_error("Listen error with max_connections = " +
                      std::to_string(g_default_max_connections));
        }
    }

    void Server::run(size_t number_threads) {
        if (number_threads == 0 || number_threads > g_max_threads) {
            number_threads = (std::thread::hardware_concurrency() == 0) ?
                    2 : std::thread::hardware_concurrency();
        }

//        std::vector<std::thread> epoll_threads(number_threads - 1, std::thread());

        std::thread epoll_threads[number_threads - 1];
        for (size_t i = 0; i < number_threads - 1; i++) {
            epoll_threads[i] = std::thread(EpollManager(*this));
        }
        EpollManager(*this).run();

        for (size_t i = 0; i < number_threads - 1; i++) {
            epoll_threads[i].join();
        }
    }

    void Server::close() {
        if (is_opened()) {
            ::shutdown(masterSocket_, SHUT_RDWR);
            masterSocket_.close();
        }
    }
}
