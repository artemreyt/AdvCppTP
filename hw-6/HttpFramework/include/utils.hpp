#ifndef TCP_UTILS_HPP
#define TCP_UTILS_HPP

#include "Connection.hpp"
#include <string>
#include <cstdint>
#include <Descriptor.hpp>
#include <sys/epoll.h>
#include <map>


namespace HttpFramework {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port);
    void createEvent(epoll_event *Event, void *ptr, uint32_t events);
    int  set_nonblock(int fd);
    std::string&    lstrip(std::string &str);
    std::string     decode_url(const std::string &url);
    void            parse_query_string(const std::string &url,
                                       std::map<std::string, std::string> &params);
}

/*
 * In order to store tcp::Connection
 * in std::unordered_set
 */
namespace std {
    template <>
    struct hash<tcp::Connection> {
        size_t operator()(const tcp::Connection &connection) const ;
    };
}



#endif //TCP_UTILS_HPP
