#include "LoggerErrors.hpp"

namespace Logger
{

    LogError::LogError(const char *err_msg) :
            text_(err_msg)
    {}

    const char *LogError::what() const noexcept
    {
        return text_.c_str();
    }

    LogRuntimeError::LogRuntimeError(const char *err_msg) :
            LogError(err_msg)
    {}


    UnknownLevelError::UnknownLevelError(Logger::t_level level_got,
                                         const char *err_msg):
        LogError(err_msg), level_got_(level_got)
    {}

    t_level UnknownLevelError::get_level() const noexcept
    {
        return level_got_;
    }
}

