#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include "Descriptor.hpp"
#include <string>

namespace tcp {

    class Connection {
    public:
        Connection(const std::string& ip, uint16_t port);
        Connection(Connection &&other) = default;
        Connection(const Connection &other) = delete;

        Connection &operator=(const Connection& other) = delete;
        Connection &operator=(Connection&& other) = default;

        ~Connection();

        void connect(const std::string& ip, uint16_t port);
        bool is_opened() const;

        size_t write(const void *data, size_t size);
        size_t read(void *data, size_t size);

        void writeExact(const void *data, size_t size);
        void readExact(void *data, size_t size);

        void close();
        void set_timeout(int sec);


        void get_connection_info(std::string *dst_ip, uint16_t *dst_port,
                    std::string *src_ip, uint16_t *src_port) const;

        friend class Server;

    protected:
        Connection() = default;

        Process::Descriptor fd_;
        std::string dst_addr_;
        std::string src_addr_;
        uint16_t src_port_;
        uint16_t dst_port_;
    };
}


#endif // TCP_CONNECTION_HPP
