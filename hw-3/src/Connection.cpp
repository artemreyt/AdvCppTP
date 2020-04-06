#include "Connection.hpp"
#include "Descriptor.hpp"
#include "Server.hpp"
#include "Errors.hpp"
#include "utils.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cerrno>
#include <utility>
#include <Process.hpp>
#include <unistd.h>
#include <cstring>

namespace tcp {

    Connection::Connection(const std::string &ip, uint16_t port) {
        connect(ip, port);
    }

    Connection::~Connection() {
        close();
    }

    void Connection::connect(const std::string& ip, uint16_t port) {
        Process::Descriptor fd(::socket(PF_INET, SOCK_STREAM, 0));

        if (fd.data() == -1) {
            throw socket_error(std::string("Socket creation error: ") + std::strerror(errno));
        }

        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        if (!inet_aton(ip.c_str(), &addr.sin_addr)) {
            throw bad_ip_address(ip);
        }
        addr.sin_port = htons(port);

        if (::connect(fd.data(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
            throw connection_error("Failure to connect to " + ip + ":" + std::to_string(port));
        }

        close();
        fd_ = std::move(fd);

        dst_addr_ = ip;
        dst_port_ = port;
        get_binded_ip_port(fd_, src_addr_, src_port_);
    }


    bool Connection::is_opened() const {
        return fd_.is_valid();
    }

    size_t Connection::write(const void *data, size_t size) {
        ssize_t bytes = ::send(fd_.data(), data, size, 0);

        if (bytes == -1)
            throw socket_error("Error to write");
        return static_cast<size_t>(bytes);
    }

    size_t Connection::read(void *data, size_t size) {
        ssize_t bytes = ::recv(fd_.data(), data, size, 0);

        if (bytes == -1)
            throw socket_error(std::string("Read error:") + std::strerror(errno));
        return static_cast<size_t>(bytes);
    }

    void Connection::writeExact(const void *data, size_t len) {
        size_t written = 0;
        while (written < len)
        {
            ssize_t bytes = write(static_cast<const char *>(data) + written, len - written);
            if (bytes <= 0) {
                throw socket_error(std::string("writeExact error: ") + std::strerror(errno));
            }
            written += bytes;
        }
    }

    void Connection::readExact(void *data, size_t len) {
        size_t was_read = 0;
        while (was_read < len)
        {
            ssize_t bytes = read(static_cast<char *>(data) + was_read, len - was_read);
            if (bytes <= 0) {
                throw socket_error(std::string("readExact error: ") + std::strerror(errno));
            }
            was_read += bytes;
        }
    }

    void Connection::close() {
        if (fd_ != -1)
            ::shutdown(fd_, SHUT_RDWR);
        fd_.close();
    }


    void Connection::set_timeout(int sec) {
        timeval timeout {.tv_sec = sec, .tv_usec = 0};
        if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw socket_error(std::string("Set timeout error:") + std::strerror(errno));
        }
    }

    void Connection::get_connection_info(std::string *dst_ip, uint16_t *dst_port,
            std::string *src_ip, uint16_t *src_port) const {
        if (dst_ip)
            *dst_ip = dst_addr_;
        if (dst_port)
            *dst_port = dst_port_;
        if (src_ip)
            *src_ip = src_addr_;
        if (src_port)
            *src_port = src_port_;
    }
}