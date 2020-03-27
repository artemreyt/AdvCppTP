#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

namespace Process
{
    class Descriptor
    {
    public:
        Descriptor() = default;

        explicit Descriptor(int fd);

        Descriptor &operator=(Descriptor &&other) noexcept;

        Descriptor &operator=(const Descriptor &other) noexcept ;

        ~Descriptor() noexcept;

        void close() noexcept;

        int get_fd() const noexcept;

        bool is_valid() const noexcept ;

        int dup2(int fd);

    private:
        int fd_ = -1;
    };
}

#endif //PROC_DESCRIPTOR_HPP
