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
#include <unistd.h>
#include <cstring>

namespace tcp {

    static const size_t MAX_BYTES_READ = 4096;

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
        ssize_t bytes = ::send(fd_.data(), data, size, MSG_NOSIGNAL);

        if (bytes == -1 && errno == EINTR)
            return write(data, size);
        if (bytes == -1)
            throw socket_error("Error to write");
        return static_cast<size_t>(bytes);
    }

    size_t Connection::read(size_t size) {
        size = (size == -1) ? MAX_BYTES_READ : size;
        std::string buffer(size + 1, '\0');
        ssize_t bytes = ::recv(fd_, buffer.data(), size, MSG_NOSIGNAL);

        if (bytes == -1)
            throw socket_error(std::string("Read error:") + std::strerror(errno));
        buffer_ += buffer;
        return static_cast<size_t>(bytes);
    }

    void Connection::writeExact(const void *data, size_t len) {
        size_t written = 0;
        while (written < len)
        {
            ssize_t bytes = write(static_cast<const char *>(data) + written, len - written);
            if (bytes <= 0) {
                close();
                throw socket_error(std::string("writeExact error: ") + std::strerror(errno));
            }
            written += bytes;
        }
    }

    void Connection::readExact(size_t len) {
        size_t was_read = 0;
        while (was_read < len)
        {
            ssize_t bytes = read(len - was_read);
            if (bytes <= 0) {
                close();
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
        if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO | SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw socket_error(std::string("Set timeout error:") + std::strerror(errno));
        }
    }

    const std::string &Connection::get_src_ip() const {
        return src_addr_;
    }

    const std::string &Connection::get_dst_ip() const {
        return dst_addr_;
    }

    const uint16_t &Connection::get_src_port() const {
        return src_port_;
    }
    const uint16_t &Connection::get_dst_port() const {
        return dst_port_;
    }

    bool Connection::operator==(const Connection &other) const {
        return this == &other;
    }

    const std::string &Connection::get_buffer() const {
        return buffer_;
    }

    void Connection::clear_buffer() {
        buffer_.clear();
    }
}