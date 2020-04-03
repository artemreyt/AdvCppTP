//
// Created by Artem Starshov on 2020-04-01.
//

#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>
#include <cstdint>
#include <memory>
#include "Descriptor.hpp"
#include "Connection.hpp"

namespace tcp {

    class Server {
    public:
        Server(const std::string &ip, uint16_t port);
        ~Server() noexcept;

        Connection accept();
        bool is_opened() const;
        void open(const std::string &ip, uint16_t port);
        void set_max_connect(int max_connect=1);
        void close();

    protected:
        Process::Descriptor fd_;
    };
}

#endif // TCP_SERVER_HPP
