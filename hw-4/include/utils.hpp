#ifndef TCP_UTILS_HPP
#define TCP_UTILS_HPP

#include <string>
#include <cstdint>
#include <Descriptor.hpp>
#include <sys/epoll.h>


namespace tcp {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port);
    void createEvent(epoll_event *Event, int fd, void *ptr, uint32_t events);
}



#endif //TCP_UTILS_HPP
