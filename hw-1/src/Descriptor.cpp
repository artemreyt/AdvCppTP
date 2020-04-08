#include "Process.hpp"
#include "Descriptor.hpp"
#include "ProcError.cpp"
#include <unistd.h>
#include <fcntl.h>

namespace Process
{
    Descriptor::Descriptor(int fd) :
            fd_{fd}
    {}

    Descriptor::Descriptor(Descriptor &&other) noexcept:
        fd_(other.fd_)
    {
        other.fd_ = -1;
    }

    Descriptor::~Descriptor() noexcept
    {
        close();
    }

    void Descriptor::close() noexcept
    {
        if (fd_ >= 0)
            ::close(fd_);
        fd_ = -1;
    }

    int Descriptor::data() const noexcept
    {
        return fd_;
    }

    Descriptor &Descriptor::operator=(Descriptor &&other) noexcept
    {
        close();
        fd_ = other.data();
        other.fd_ = -1;
        return *this;
    }

    bool Descriptor::is_valid() const noexcept
    {
        return fd_ >= 0 && fcntl(fd_, F_GETFL) != -1;
    }

    Descriptor &Descriptor::operator=(const Descriptor &other) noexcept
    {
        close();
        fd_ = dup(other.fd_);
        return *this;
    }

    int Descriptor::dup2(int fd)
    {
        int retv;

        if ((retv = ::dup2(fd_, fd)) == -1)
            throw DupError();
        return retv;
    }

    Descriptor::operator int() const noexcept
    {
        return fd_;
    }

}
