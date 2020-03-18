#include "Process.hpp"
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

namespace artemreyt
{
    Process::Process(const std::string &path)
    {
        std::vector<char *> params = parse_params(path);
        int pipefd_in[2], pipefd_out[2];
        get_pipe(pipefd_in);
        get_pipe(pipefd_out);

        child_pid_ = fork();
        switch (child_pid_)
        {
            case -1:
                throw std::runtime_error("Isn't able to fork");
            case 0:
            {
                ::close(pipefd_in[1]);
                ::close(pipefd_out[0]);
                dup2(pipefd_in[0], STDIN_FILENO);
                dup2(pipefd_out[1], STDOUT_FILENO);

                if (execvp(params[0], params.data()))
                {
                    std::string msg = "Isn't able to execute " + path;
                    throw std::runtime_error(msg.c_str());
                }
            }
            default:
                for (auto &par: params)
                    ::free(par);
                params.clear();

                child_stdin_ = pipefd_in[1];
                child_stdout_ = pipefd_out[0];
                ::close(pipefd_in[0]);
                ::close(pipefd_out[1]);
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
        return ::write(child_stdin_, data, len);
    }

    size_t Process::read(void *data, size_t len)
    {
        return ::read(child_stdout_, data, len);
    }

    void Process::closeStdin()
    {
        ::close(child_stdin_);
    }

    void Process::close()
    {
        closeStdin();
        ::close(child_stdin_);
    }

    void Process::writeExact(const void *data, size_t len)
    {
        size_t written = 0;
        while (written < len)
        {
            ssize_t bytes = write((char *)data + written, len - written);
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
            ssize_t bytes = read((char *) data + was_read, len - was_read);
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
        return fcntl(child_stdout_, F_GETFL) != -1;
    }

    int Process::getStdin() const
    {
        return child_stdin_;
    }

    int Process::getStdout() const
    {
        return child_stdout_;
    }
}



