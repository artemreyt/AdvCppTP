#ifndef PROC_PROTOCOL_HPP
#define PROC_PROTOCOL_HPP

#include "Process.hpp"
#include <string>

int protocolRead(Process &proc, std::string &buffer);
int protocolRead(int fd, std::string &buffer);

int protocolWrite(Process &proc, const std::string &data);
int protocolWrite(int fd, const std::string &data);


#endif //PROC_PROTOCOL_HPP
