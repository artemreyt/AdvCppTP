#include "Server.hpp"
#include "Errors.hpp"
#include "Descriptor.hpp"
#include "utils.hpp"
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>

namespace tcp {

    static const int g_default_max_connections = 5;

    Server::Server(const std::string &ip, uint16_t port) {
        open(ip, port);
    }

    Server::~Server() noexcept {
        close();
    }

    Connection Server::accept() {
        Connection con;
        sockaddr_in client_addr {};
        socklen_t len = sizeof(client_addr);
        Process::Descriptor connect_fd(::accept(fd_, reinterpret_cast<sockaddr *>(&client_addr), &len));

        if (connect_fd.data() == -1) {
            throw accept_error(std::string("Accept error: ") + std::strerror(errno));
        }
        get_binded_ip_port(connect_fd.data(), con.src_addr_, con.src_port_);
        con.dst_addr_ = ::inet_ntoa(client_addr.sin_addr);
        con.dst_port_ = ntohs(client_addr.sin_port);
        con.fd_ = std::move(connect_fd);
        return con;
    }

    bool Server::is_opened() const {
        return fd_.is_valid();
    }

    void Server::open(const std::string &ip, uint16_t port) {
        Process::Descriptor fd(::socket(PF_INET, SOCK_STREAM, 0));
        if (fd.data() == -1) {
            throw socket_error(std::string("Socket creation error: ") + std::strerror(errno));
        }

        int optval = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

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

        fd_ = std::move(fd);
    }

    void Server::set_max_connect(int max_connect) {
        if (is_opened()) {
            ::listen(fd_, max_connect);
        }
    }

    void Server::close() {
        if (is_opened()) {
            ::shutdown(fd_, SHUT_RDWR);
            fd_.close();
        }
    }
}
