#ifndef PROC_PROTOCOL_HPP
#define PROC_PROTOCOL_HPP

#include <string>

namespace Protocol
{
    ssize_t read(int fd, std::string &buffer);
    ssize_t write(int fd, const std::string &data);
}

#endif //PROC_PROTOCOL_HPP
