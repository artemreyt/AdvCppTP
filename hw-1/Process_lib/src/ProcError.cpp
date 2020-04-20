#include "ProcError.hpp"

namespace Process
{
    ProcError::ProcError(const std::string &msg):
        msg_(msg) {}

    const char *ProcError::what() const noexcept
    {
        return msg_.c_str();
    }

    PipeError::PipeError(const std::string &msg):
        ProcError(msg) {}

    DupError::DupError(const std::string &msg):
        ProcError(msg) {}

    WriteError::WriteError(const std::string &msg):
        ProcError(msg) {}

    ReadError::ReadError(const std::string &msg):
        ProcError(msg) {}
}
