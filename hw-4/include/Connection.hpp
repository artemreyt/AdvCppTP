#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include "Descriptor.hpp"
#include <string>

namespace tcp {

    class Connection {
    public:
        Connection() = default;
        Connection(const std::string &ip, uint16_t port);
        Connection(Connection &&other) = default;
        Connection(const Connection &other) = delete;

        Connection &operator=(const Connection &other) = delete;
        Connection &operator=(Connection &&other) = default;

        bool operator==(const Connection &other) const;
        ~Connection();

        void connect(const std::string &ip, uint16_t port);
        [[nodiscard ]] bool is_opened() const;

        size_t write(const void *data, size_t size);
        size_t read(size_t size=-1);

        void writeExact(const void *data, size_t size);
        void readExact(size_t size);

        void close();
        void set_timeout(int sec);

        [[ nodiscard ]] const std::string &get_src_ip() const;
        [[ nodiscard ]] const std::string &get_dst_ip() const;
        [[ nodiscard ]] const uint16_t &get_src_port() const;
        [[ nodiscard ]] const uint16_t &get_dst_port() const;
        [[ nodiscard ]] std::string &get_buffer() ;
        [[ nodiscard ]] const Descriptor::Descriptor &get_fd() const;
        void clear_buffer();

        friend class Server;

    protected:
        Descriptor::Descriptor fd_;
        std::string dst_addr_;
        std::string src_addr_;
        uint16_t src_port_{};
        uint16_t dst_port_{};
        std::string buffer_;
    };
}

#endif // TCP_CONNECTION_HPP
