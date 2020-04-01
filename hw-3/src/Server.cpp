#include "Server.hpp"
#include "Errors.hpp"
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tcp {

    static const int g_default_max_connections = 5;

    Server::Server(const std::string &ip, uint16_t port): fd_(-1) {
        open(ip, port);
    }

    Server::~Server() noexcept {
        if (is_opened()) {
            close();
        }
    }

    Connection Server::accept() {

    }

    bool Server::is_opened() const {
        return fd_ != -1;
    }

    void Server::open(const std::string &ip, uint16_t port) {
        int fd = (::socket(PF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            throw socket_error(std::string("Socket creation error: ") + std::strerror(errno));
        }

        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        if (!inet_aton(ip.c_str(), &addr.sin_addr)) {
            ::close(fd);
            throw bad_ip_address(ip);
        }
        addr.sin_port = port;

        if (::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
            ::close(fd);
            throw socket_error(std::string("Bind error: ") + std::strerror(errno));
        }

        if (::listen(fd, g_default_max_connections) == -1) {
            ::close(fd);
            throw socket_error("Listen error with max_connections = " +
                               std::to_string(g_default_max_connections));
        }

        if (is_opened()) {
            close();
        }
        fd_ = fd;
    }

    void Server::set_max_connect(int max_connect) {
        if (is_opened()) {
            ::listen(fd_, max_connect);
        }
    }

    void Server::close() {
        ::close(fd_);
        fd_ = -1;
    }
}
