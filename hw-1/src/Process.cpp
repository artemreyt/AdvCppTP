#include "Process.hpp"
#include "Descriptor.hpp"
#include "ProcError.hpp"
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
                pipe_in_write.close();
                pipe_out_read.close();

                pipe_in_read.dup2(STDIN_FILENO);
                pipe_out_write.dup2(STDOUT_FILENO);

                pipe_in_read.close();
                pipe_out_write.close();
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

    size_t Process::write(const void *data, size_t len)
    {
        ssize_t bytes = ::write(child_stdin_.data(), data, len);

        if (bytes == -1)
            throw WriteError();
        return static_cast<size_t>(bytes);
    }

    size_t Process::read(void *data, size_t len)
    {
        ssize_t bytes = ::read(child_stdout_.data(), data, len);

        if (bytes == -1)
            throw ReadError();
        return static_cast<size_t>(bytes);
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
                    throw WriteError("Access to file is denied");
                else
                    throw WriteError("Invalid file number");
            }
            else if (bytes == 0)
                throw WriteError("Can't write to file");
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
                    throw ReadError("Access to file is denied");
                else
                    throw ReadError("Invalid file number");
            }
            else if (bytes == 0)
                throw ReadError("EOF founded");
            was_read += bytes;
        }
    }

    bool Process::isReadable() const
    {
        return child_stdout_.is_valid();
    }

    int Process::getStdin() const
    {
        return child_stdin_.data();
    }

    int Process::getStdout() const
    {
        return child_stdout_.data();
    }
}



