#ifndef ADVCPPTP_UTILS_HPP
#define ADVCPPTP_UTILS_HPP

#include <type_traits>

namespace utils {

    template<typename RetType, typename U, typename Alloc>
    std::enable_if_t<!std::is_pod_v<RetType>, RetType>
    get_object_with_allocator_if_enable(const U &obj, const Alloc &alloc)
    {
        static_assert(std::uses_allocator_v<RetType, Alloc>, "Not POD type should use Alloc");
        static_assert(std::is_constructible_v<RetType, U, Alloc>, "RetType should be constructible from {U, Alloc}");

        return RetType(obj, alloc);
    }

    template<typename U, typename Alloc>
    std::enable_if_t<std::is_pod_v<U>, U>
    get_object_with_allocator_if_enable(const U &obj, const Alloc &alloc)
    {
        return obj;
    }

    template<typename U, typename Alloc>
    std::enable_if_t<!std::is_pod_v<U>, U>
    get_default_object_with_allocator(const Alloc &alloc)
    {
        static_assert(std::uses_allocator_v<U, Alloc>, "Not POD type should use Alloc");
        static_assert(std::is_constructible_v<U, Alloc>, "U should be constructible from Alloc");

        return U(alloc);
    }

    template<typename U, typename Alloc>
    std::enable_if_t<std::is_pod_v<U>, U>
    get_default_object_with_allocator(const Alloc &alloc) {
        return U();
    }
}
#endif //ADVCPPTP_UTILS_HPP
