#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include "Connection.hpp"
#include <string>
#include <cstdint>
#include <Descriptor.hpp>
#include <sys/epoll.h>
#include <unordered_map>


namespace HttpFramework {
    namespace utils {
        void get_binded_ip_port(int fd, std::string &ip, uint16_t &port);

        void createEvent(epoll_event *Event, void *ptr, uint32_t events);

        std::string &lstrip(std::string &str);

        std::string decode_url(const std::string &url);

        void parse_query_string(std::string_view url,
                                std::unordered_map<std::string, std::string> &params);

        std::string string_thread_id();

        std::string to_lowercase(const std::string &str);

    }
}
#endif //HTTP_UTILS_HPP
