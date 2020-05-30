#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include "Connection.hpp"
#include <string>
#include <cstdint>
#include <Descriptor.hpp>
#include <sys/epoll.h>
#include <unordered_map>


namespace HttpFramework {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port);
    void createEvent(epoll_event *Event, void *ptr, uint32_t events);
    int  set_nonblock(int fd);
    std::string&    lstrip(std::string &str);
    std::string     decode_url(const std::string &url);
    void            parse_query_string(const std::string &url,
                                       std::unordered_map<std::string, std::string> &params);
    std::string     string_thread_id();
}

#endif //HTTP_UTILS_HPP
