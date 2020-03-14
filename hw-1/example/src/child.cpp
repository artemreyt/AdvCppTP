#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <cctype>
#include <string>
#include "Protocol.hpp"


static std::string upper(const std::string &str)
{
    std::string new_str(str);
    for (auto &c: new_str)
        c = toupper(c);
    return new_str;
}

int main()
{
    std::string buffer;

    while (!feof(stdin))
    {
        if (protocolRead(STDIN_FILENO, buffer) == -1)
            break;
        if (protocolWrite(STDOUT_FILENO, upper(buffer)) == -1)
            break;
        buffer.clear();
    }
    return 0;
}
