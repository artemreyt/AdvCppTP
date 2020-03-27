#ifndef LOGGERERRORS_HPP
#define LOGGERERRORS_HPP

#include "BaseLogger.hpp"

namespace Logger
{
    class LogError: std::exception
    {
    public:
        LogError(const char *err_msg);
        const char *what() const noexcept override;

    protected:
        std::string text_;
    };

    class LogRuntimeError: public LogError
    {
    public:
        LogRuntimeError(const char *err_msg);
    };


    class UnknownLevelError: public LogError
    {
    public:
        UnknownLevelError(Logger::t_level level_got_,
                           const char *msg="Unknown Log Level got.");
        t_level get_level() const noexcept;

    private:
        t_level level_got_;
    };
}


#endif // LOGGERERRORS_HPP
