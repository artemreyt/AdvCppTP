#include "Process.hpp"
#include "Descriptor.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <csignal>
#include "utils.hpp"
#include <utility>

namespace Process
{
    Process::Process(const std::string &path)
    {
        int pipefd[2];

        get_pipe(pipefd);
        Descriptor pipe_in_read(pipefd[0]);
        Descriptor pipe_in_write(pipefd[1]);

        get_pipe(pipefd);
        Descriptor pipe_out_read(pipefd[0]);
        Descriptor pipe_out_write(pipefd[1]);

        child_pid_ = fork();
        switch (child_pid_)
        {
            case -1:
                throw std::runtime_error("Isn't able to fork");
            case 0:
            {
                pipe_in_read.dup2(STDIN_FILENO);
                pipe_out_write.dup2(STDOUT_FILENO);

                if (try_exec(path))
                {
                    std::string msg = "Isn't able to execute " + path;
                    throw std::runtime_error(msg.c_str());
                }
            }
            default:
                child_stdin_ = std::move(pipe_in_write);
                child_stdout_ = std::move(pipe_out_read);
        }
    }

    Process::~Process()
    {
        close();
        kill(child_pid_, SIGTERM);
        waitpid(child_pid_, 0, 0);
    }

    ssize_t Process::write(const void *data, size_t len)
    {
        return ::write(child_stdin_.get_fd(), data, len);
    }

    ssize_t Process::read(void *data, size_t len)
    {
        return ::read(child_stdout_.get_fd(), data, len);
    }

    void Process::closeStdin()
    {
        child_stdin_.close();
    }

    void Process::close()
    {
        closeStdin();
        child_stdout_.close();
    }

    void Process::writeExact(const void *data, size_t len)
    {
        size_t written = 0;
        while (written < len)
        {
            ssize_t bytes = write(static_cast<const char *>(data) + written, len - written);
            if (bytes == -1)
            {
                if (errno == EACCES)
                    throw std::runtime_error("Access to file is denied");
                else
                    throw std::runtime_error("Invalid file number");
            }
            else if (bytes == 0)
                throw std::runtime_error("Can't write to file");
            written += bytes;
        }
    }

    void Process::readExact(void *data, size_t len)
    {
        size_t was_read = 0;
        while (was_read < len)
        {
            ssize_t bytes = read(static_cast<char *>(data) + was_read, len - was_read);
            if (bytes == -1)
            {
                if (errno == EACCES)
                    throw std::runtime_error("Access to file is denied");
                else
                    throw std::runtime_error("Invalid file number");
            }
            else if (bytes == 0)
                throw std::runtime_error("EOF founded");
            was_read += bytes;
        }
    }

    bool Process::isReadable() const
    {
        return child_stdout_.is_valid();
    }

    int Process::getStdin() const
    {
        return child_stdin_.get_fd();
    }

    int Process::getStdout() const
    {
        return child_stdout_.get_fd();
    }
}



