#include "utils.hpp"
#include "ProcError.hpp"
#include <string>
#include <vector>
#include <csignal>
#include <unistd.h>
#include <stdexcept>

int try_exec(const std::string &path)
{
    std::vector<std::string> params_cxx = parse_params(path);
    std::vector<char *> params_c;
    for (std::string& str: params_cxx)
        params_c.push_back(str.data());
    params_c.push_back(nullptr);

    return execvp(params_c[0], params_c.data());
}


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
        throw Process::PipeError();
}

