#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

namespace Descriptor
{
    class Descriptor
    {
    public:
        Descriptor() = default;

        explicit Descriptor(int fd);

        Descriptor(Descriptor &&other) noexcept ;

        // TODO !!!
        Descriptor(const Descriptor &other) = default;

        Descriptor &operator=(Descriptor &&other) noexcept;

        Descriptor &operator=(const Descriptor &other) noexcept ;

        ~Descriptor() noexcept;

        void close() noexcept;

        operator int() const noexcept ;

        int data() const noexcept;

        bool is_valid() const noexcept ;

        int dup2(int fd);

    private:
        int fd_ = -1;
    };
}

#endif //PROC_DESCRIPTOR_HPP
