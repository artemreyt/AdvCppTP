#include "process.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <exception>
#include <cstring>
#include <csignal>
#include <cerrno>
#include <fcntl.h>

static const auto npos = std::string::npos;
static const size_t msg_len = 256;

static std::vector<char *> parse_params(const std::string& path)
{
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

static	void get_pipe(int pipefd[2])
{
    if (pipe(pipefd) < 0)
        throw std::runtime_error("Pipe error");
}

Process::Process(const std::string& path):
    params(parse_params(path))
{
    int pipefd_in[2], pipefd_out[2];
    get_pipe(pipefd_in);
    get_pipe(pipefd_out);

    cpid = fork();
    switch (cpid)
    {
        case -1:
            throw std::runtime_error("Isn't able to fork");
        case 0:
        {
            ::close(pipefd_in[1]);
            ::close(pipefd_out[0]);
            dup2(pipefd_in[0], STDIN_FILENO);
            dup2(pipefd_out[1], STDOUT_FILENO);
            execvp(params[0], params.data());

            char msg[msg_len];
            snprintf(msg, msg_len - 1, "Isn't able to execute %s", path.c_str());
            throw std::runtime_error(msg);
            break;
        }
        default:
            child_stdin = pipefd_in[1];
            child_stdout = pipefd_out[0];
            ::close(pipefd_in[0]);
            ::close(pipefd_out[1]);
            break;
    }
}

Process::~Process()
{
    close();
    for (auto &param: params)
        ::free(param);
    kill(cpid, SIGTERM);
}

size_t Process::write(const void* data, size_t len)
{
    return ::write(child_stdin, data, len); 
}

size_t Process::read(void* data, size_t len)
{
    return ::read(child_stdout, data, len);
}

void Process::closeStdin()
{
    ::close(child_stdin);
}

void Process::close()
{
    ::close(child_stdout);
    ::close(child_stdin);
}

void Process::writeExact(const void* data, size_t len)
{
    size_t written = 0;
    while (written < len)
    {
        ssize_t bytes = this->write((char *)data + written, len - written);
        if (bytes == -1)
        {
            if (errno == EACCES)
                throw std::runtime_error("Access to file is denied");
            else
                throw std::runtime_error("Invalid file number");
        }
        written += bytes;
    }
}

void Process::readExact(void* data, size_t len)
{
    size_t was_read = 0;
    while (was_read < len)
    {
        ssize_t bytes = this->read((char *)data + was_read, len - was_read);
        if (bytes == -1)
        {
            if (errno == EACCES)
                throw std::runtime_error("Access to file is denied");
            else
                throw std::runtime_error("Invalid file number");
        }
        was_read += bytes;
    }
}

bool Process::isReadable() const
{
    return fcntl(child_stdout, F_GETFL) != -1;
}
