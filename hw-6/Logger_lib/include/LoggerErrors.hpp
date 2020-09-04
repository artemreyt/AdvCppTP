#ifndef LOGGERERRORS_HPP
#define LOGGERERRORS_HPP

#include "BaseLogger.hpp"

namespace Logger
{
    class LogError: std::exception
    {
    public:
        explicit LogError(const std::string &err_msg);
        const char *what() const noexcept override;

    protected:
        std::string text_;
    };

    class LogRuntimeError: public LogError
    {
    public:
        explicit LogRuntimeError(const std::string &err_msg);
    };
}


#endif // LOGGERERRORS_HPP
