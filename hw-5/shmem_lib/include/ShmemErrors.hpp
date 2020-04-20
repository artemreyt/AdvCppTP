#ifndef SHMEMERRORS_HPP
#define SHMEMERRORS_HPP

#include <stdexcept>
#include <string>

namespace shmem {

    class ShmemError : public std::exception {
    public:
        explicit ShmemError(std::string msg = "shmem::error");

        const char *what() const noexcept override;

    protected:
        std::string msg_;
    };

    class MMapError: public ShmemError {
    public:
        explicit MMapError(std::string msg="Mmap Error");
    };
}
#endif //ADVCPPTP_SHMEMERRORS_HPP
