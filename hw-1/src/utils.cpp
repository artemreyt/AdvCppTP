#include "utils.hpp"
#include <string>
#include <vector>
#include <csignal>
#include <unistd.h>

std::vector<char *> parse_params(const std::string& path)
{
    const size_t npos = std::string::npos;
    std::vector<char *> params;

    size_t start = 0;
    while (true)
    {
        size_t end = path.find(' ', start);
        params.push_back(::strdup(path.substr(start, end != npos ? end - start : end).c_str()));
        if (end == npos)
            break;
        start = end + 1;
    }
    params.push_back(NULL);
    return params;
}


void get_pipe(int pipefd[2])
{
    if (pipe(pipefd) < 0)
        throw std::runtime_error("Pipe error");
}

