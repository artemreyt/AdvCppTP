#include "Server.hpp"
#include "Errors.hpp"
#include "Descriptor.hpp"
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tcp {

    static const int g_default_max_connections = SOMAXCONN;

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

    void Server::set_max_connect(int max_connect) {
        if (::listen(masterSocket_, max_connect) == -1) {
            throw socket_error("Listen error with max_connections = " +
                      std::to_string(g_default_max_connections));
        }
    }

    void Server::eventLoop() {
        EpollManager manager(*this);
        manager.run();
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
