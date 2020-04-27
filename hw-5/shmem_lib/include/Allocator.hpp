#ifndef SHMEM_ALLOCATOR_HPP
#define SHMEM_ALLOCATOR_HPP

#include <memory>
#include <semaphore.h>

namespace shmem
{

    struct AllocState {
        char *start;
        char *end;
    };


    template <typename T>
    struct Allocator {
    public:
        using value_type = T;
        using  pointer	= T*;
        using const_pointer	= const T*;
        using reference = T&;

        explicit Allocator(AllocState *state);
        Allocator(const Allocator& other) = default;

        template <class U>
        Allocator (const Allocator<U>& alloc)  {
            state_ = alloc.state_;
        }

        template <typename U>
        bool operator!=(const Allocator<U> &other) {
            return state_ != other.state_;
        }

        T *allocate(std::size_t n);
        void deallocate(T *p, std::size_t n);

        AllocState *state_;
    };

    template <typename T>
    Allocator<T>::Allocator(AllocState *state):
        state_(state){
    }

    template <typename T>
    T* Allocator<T>::allocate(std::size_t n) {
        char *result = state_->start;

        if (result + sizeof(T) * n > state_->end) {
            throw std::bad_alloc{};
        }

        state_->start += sizeof(T) * n;
        return reinterpret_cast<T*>(result);
    }

    template <typename T>
    void Allocator<T>::deallocate(T *p, std::size_t n) {
        if (reinterpret_cast<char *>(p) + sizeof(T) * n == state_->start)
            state_->start = reinterpret_cast<char *>(p);
    }


    template <class CharT>
    using string = std::basic_string<CharT, std::char_traits<CharT>, Allocator<CharT>>;
}

#endif