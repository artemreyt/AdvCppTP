#ifndef PROCERROR_HPP
#define PROCERROR_HPP

#include <stdexcept>
#include <string>

namespace Process
{

    class ProcError: public std::exception
    {
    public:
        const char *what() const noexcept override ;

    protected:
        const std::string msg_;
    };

    class PipeError: public ProcError
    {
    public:
        PipeError() = default;

    protected:
       const std::string msg_ = "Pipe creation error";
    };

    class DupError: public ProcError
    {
    public:
        DupError() = default;

    protected:
        const std::string msg_ = "Dup error";
    };

}

#endif //PROC_PROCERROR_HPP
