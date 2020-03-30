#ifndef PROCERROR_HPP
#define PROCERROR_HPP

#include <stdexcept>
#include <string>

namespace Process
{

    class ProcError: public std::exception
    {
    public:
        ProcError() = default;
        explicit ProcError(const std::string &msg);
        const char *what() const noexcept override ;

    protected:
        const std::string msg_ = "Process::Process error";
    };

    class PipeError: public ProcError
    {
    public:
        explicit PipeError(const std::string &msg="Pipe creation error");
    };

    class DupError: public ProcError
    {
    public:
        explicit DupError(const std::string &msg="Dup error");
    };

    class WriteError: public ProcError
    {
    public:
        explicit WriteError(const std::string &msg="Write error");
    };

    class ReadError: public ProcError
    {
    public:
        explicit ReadError(const std::string &msg="Read error");
    };
}

#endif //PROC_PROCERROR_HPP
