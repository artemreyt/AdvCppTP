#include "LoggerErrors.hpp"

namespace Logger
{

    LogError::LogError(const std::string &err_msg) :
            text_(err_msg)
    {}

    const char *LogError::what() const noexcept
    {
        return text_.c_str();
    }

    LogRuntimeError::LogRuntimeError(const std::string &err_msg) :
            LogError(err_msg)
    {}
}

