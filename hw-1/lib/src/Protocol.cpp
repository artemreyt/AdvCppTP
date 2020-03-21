#include <string>
#include <cstdio>
#include <unistd.h>
#include "Protocol.hpp"
#include <iostream>

int Protocol::read(int fd, std::string &buffer)
{
    size_t size = 0;
    size_t was_read = 0;

    while (was_read < sizeof(size))
    {
        size_t bytes = ::read(fd, &size + was_read, sizeof(size) - was_read);
        if (bytes <= 0)
            return -1;
        was_read += bytes;
    }

    buffer.clear();
    buffer.resize(size);
    was_read = 0;
    while (was_read < size)
    {
        size_t bytes = ::read(fd, buffer.data() + was_read, size - was_read);
        if (bytes <= 0)
            return -1;
        was_read += bytes;
    }
    return 0;
}

int Protocol::write(int fd, const std::string &data)
{
    size_t size = data.size();

    std::string buffer(reinterpret_cast<char *>(&size), sizeof(size));
    buffer += data;
    size_t was_written = 0;
    while (was_written < buffer.size())
    {
        size_t bytes = ::write(fd, buffer.data() + was_written, buffer.size() - was_written);
        if (bytes <= 0)
            return -1;
        was_written += bytes;
    }
    return 0;
}


