#include "utils.hpp"
#include "Errors.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <thread>


namespace HttpFramework {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port) {
        sockaddr_in addr {};
        socklen_t len = sizeof(addr);

        int retv = getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &len);
        if (retv == -1) {
            throw tcp_error(std::string("Fail to get socket data: ") + std::strerror(errno));
        }
        ip = ::inet_ntoa(addr.sin_addr);
        port = ntohs(addr.sin_port);
    }

    void createEvent(epoll_event *Event, void *ptr, uint32_t events) {
        Event->data.ptr = ptr;
        Event->events = events;
    }

    int  set_nonblock(int fd) {
        int flags;
    #if defined(O_NONBLOCK)
        if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
            flags = 0;
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    #else
        flags = 1;
        return ioctl(fd, FIOBIO, &flags);
    #endif
    }

    std::string& lstrip(std::string &str) {
        str.erase(0, str.find_first_not_of(" \n\r\t"));
        return str;
    }

    std::string decode_url(const std::string &url) {
        std::string res_str;

        const char *c_str = url.c_str();
        while (*c_str) {
            if (*c_str == '%' && c_str[1] && c_str[2]) {
                res_str += std::strtoul(std::string(c_str + 1, 2).c_str(), nullptr, 16);
                c_str += 3;
            } else {
                res_str += *c_str;
                c_str++;
            }
        }
        return res_str;
    }

    void parse_query_string(const std::string &query_string, std::map<std::string, std::string> &params) {
        std::string decode_query = decode_url(query_string);

        size_t start = 0;
        while (start != std::string::npos) {
            size_t delim_pos = decode_query.find('=');
            auto key = decode_query.substr(start, delim_pos);
            start = decode_query.find('&');
            auto value = decode_query.substr(delim_pos, start);
            params.emplace(std::move(key), std::move(value));
        }
    }

    std::string string_thread_id() {
        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        return ss.str();
    }
}

