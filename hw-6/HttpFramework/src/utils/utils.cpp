#include "utils/utils.hpp"
#include "Errors.hpp"
#include "constants.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <string_view>
#include <cctype>


namespace HttpFramework {

namespace utils {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port) {
        sockaddr_in addr{};
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

    std::string &lstrip(std::string &str) {
        str.erase(0, str.find_first_not_of(" \n\r\t"));
        return str;
    }

    std::string decode_url(std::string_view encoded_str) {
        std::string result;
        while (!encoded_str.empty()) {
            switch (encoded_str.front()) {
                case '%': {
                    if (encoded_str.size() < 3) {
                        return std::string();
                    }

                    const char hex[] = {encoded_str[1], encoded_str[2], '\0'};
                    result += static_cast<char>(std::strtoul(hex, nullptr, 16));

                    encoded_str.remove_prefix(3);
                    break;
                }
                default: {
                    result += encoded_str.front();
                    encoded_str.remove_prefix(1);
                }
            }
        }
        return result;
    }

    void parse_query_string(std::string_view query_string,
                            std::unordered_map<std::string, std::string> &params) {
        while (!query_string.empty()) {
            if (query_string.front() == '&') {
                query_string.remove_prefix(1);
            }

            auto line = query_string.substr(0, query_string.find('&'));

            size_t header_len = line.find('=');

            std::string_view header = line.substr(0, header_len);
            std::string_view value = line.substr(header_len + 1);

            params.emplace(decode_url(header), decode_url(value));
            query_string.remove_prefix(line.size());
        }
    }

    std::string string_thread_id() {
        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        return ss.str();
    }

    std::string to_lowercase(std::string_view str) {
        std::string lowercase;
        lowercase.reserve(str.size());

        for (char c: str) {
            lowercase += static_cast<char>(::tolower(c));
        }

        return lowercase;
    }

    std::string to_string(constants::RequestMethod method) {
        if (method == constants::RequestMethod::GET) {
            return "GET";
        } else if (method == constants::RequestMethod::POST) {
            return "POST";
        } else {
            return std::string();
        }
    }

    std::string to_string(constants::HttpVersion version) {
        if (version == constants::HttpVersion::V1_0) {
            return "HTTP/1.0";
        } else if (version == constants::HttpVersion::V1_1) {
            return "HTTP/1.1";
        } else {
            return std::string();
        }
    }

    std::string_view rtrim(std::string_view v) {
        ssize_t i;
        for (i = v.size() - 1; i > 0; --i) {
            if (!::strchr(constants::strings::whitespaces, v[i])) {
                break;
            }
        }
        v.remove_suffix(v.size() - i - 1);
        return v;
    }
}
}

