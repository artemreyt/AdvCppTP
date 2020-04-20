#include "ShmemErrors.hpp"

namespace shmem {
    ShmemError::ShmemError(std::string msg): msg_(std::move(msg)) {
    }

    const char *ShmemError::what() const noexcept  {
        return msg_.c_str();
    }

    MMapError::MMapError(std::string msg): ShmemError(std::move(msg)) {
    }
}

