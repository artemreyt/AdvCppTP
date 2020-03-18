#ifndef PROC_PROTOCOL_HPP
#define PROC_PROTOCOL_HPP

#include <string>

namespace Protocol
{
    int read(int fd, std::string &buffer);
    int write(int fd, const std::string &data);
}

#endif //PROC_PROTOCOL_HPP
