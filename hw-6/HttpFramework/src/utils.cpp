#include "utils.hpp"
#include "Errors.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>


namespace HttpFramework {
    void get_binded_ip_port(int fd, std::string &ip, uint16_t &port) {
        sockaddr_in addr {};
        socklen_t len = sizeof(addr);

        int retv = getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &len);
        if (retv == -1) {
            throw socket_error(std::string("Fail to get socket data: ") + std::strerror(errno));
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
}

size_t
std::hash<tcp::Connection>::operator()(const tcp::Connection &connection) const {
    size_t h1 = std::hash<std::string>()(connection.get_dst_ip());

    /* if port will change type */
    size_t h2 = std::hash<std::remove_const_t<
                    std::remove_reference_t<decltype(connection.get_dst_port())>
                    >>()(connection.get_dst_port());
    return h1 ^ (h2 << 1);
}

