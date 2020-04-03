#ifndef TCP_UTILS_HPP
#define TCP_UTILS_HPP

#include <string>
#include <cstdint>
#include <Descriptor.hpp>


namespace tcp {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port);
}



#endif //TCP_UTILS_HPP
