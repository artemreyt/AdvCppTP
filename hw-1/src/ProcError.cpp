#include "ProcError.hpp"

namespace Process
{
    const char *ProcError::what() const noexcept
    {
        return msg_.c_str();
    }
}
