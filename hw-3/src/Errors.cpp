#include "Errors.hpp"
#include <utility>

namespace tcp {

    error::error(std::string msg): msg_(std::move(msg)) {
    }

    const char *error::what() const noexcept { return msg_.c_str(); }


    socket_error::socket_error(std::string msg) : error(std::move(msg)) {
    }

    bad_ip_address::bad_ip_address(std::string ip):
            socket_error(ip + " is bas IP address"), ip_(std::move(ip)) {
    }

    const std::string &bad_ip_address::get_ip() const {
        return ip_;
    }

    connection_error::connection_error(std::string msg):
        error(std::move(msg)){
    }

    accept_error::accept_error(std::string msg) : error(std::move(msg)) {
    }
}