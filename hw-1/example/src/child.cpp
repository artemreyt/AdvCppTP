#include <unistd.h>
#include <cctype>
#include <string>
#include "Protocol.hpp"


static std::string upper(const std::string &str)
{
    std::string new_str(str);
    for (char &c: new_str)
        c = toupper(c);
    return new_str;
}

int main()
{
    std::string buffer;

    while (!feof(stdin))
    {
        if (Protocol::read(STDIN_FILENO, buffer) == -1)
            break;
        if (Protocol::write(STDOUT_FILENO, upper(buffer)) == -1)
            break;
        buffer.clear();
    }
    return 0;
}
