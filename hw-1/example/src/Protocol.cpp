#include "Process.hpp"
#include <string>
#include <cstdio>
#include <exception>
#include <unistd.h>

int protocolRead(Process &proc, std::string &buffer)
{
    try
    {
        size_t size;
        char *message;

        proc.readExact(&size, sizeof(size));
        message = new char[size + 1];
        message[size] = '\0';
        proc.readExact(message, size);
        buffer = message;
        delete[] message;
        return 0;
    } catch (std::runtime_error &err) {
        return -1;
    }
}

int protocolRead(int fd, std::string &buffer)
{
    size_t size;
    char *message;

    if (read(fd, &size, sizeof(size)) <= 0)
        return -1;
    message = new char[size + 1];
    message[size] = '\0';
    if (read(fd, message, size) != size)
        return -1;
    buffer = message;
    return 0;
}


int protocolWrite(Process &proc, const std::string &data)
{
    try
    {
        size_t size = data.size();
        proc.writeExact(&size, sizeof(size));
        proc.writeExact(data.c_str(), data.size());
        return 0;
    } catch (std::runtime_error &err) {
        return -1;
    }
}

int protocolWrite(int fd, const std::string &data)
{
    size_t size = data.size();
    if (write(fd, &size, sizeof(size)) <= 0)
        return -1;
    if (write(fd, data.c_str(), data.size()) != size)
        return -1;
    return 0;
}


