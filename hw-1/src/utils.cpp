#include "utils.hpp"
#include <string>
#include <vector>
#include <csignal>
#include <unistd.h>
#include <stdexcept>

std::vector<std::string> parse_params(const std::string& path)
{
    const size_t npos = std::string::npos;
    std::vector<std::string> params;

    size_t start = 0;
    while (true)
    {
        size_t end = path.find(' ', start);
        params.push_back(path.substr(start, end != npos ? end - start : end));
        if (end == npos)
            break;
        start = end + 1;
    }
    return params;
}


void get_pipe(int pipefd[2])
{
    if (pipe(pipefd) < 0)
        throw std::runtime_error("Pipe error");
}

