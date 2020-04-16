#include "Allocator.hpp"
#include "SemLock.hpp"
#include "ShmemErrors.hpp"
#include "Utils.hpp"
#include <map>
#include <utility>
#include <semaphore.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>
#include <type_traits>

using utils::SemLock;

namespace shmem {

    const size_t MMAP_SIZE = 8192;

    template <typename K, typename T>
    class Map {
        using allocator_type = Allocator<std::pair<const K, T>>;
        using MapType = std::map<K, T, std::less<K>, allocator_type>;

        MapType         *mapObject_;
        allocator_type  allocator_;
        AllocState      *state_;
        sem_t           *sem_;

    public:

        Map():
            state_(reinterpret_cast<AllocState*>(::mmap(nullptr,
                                                        MMAP_SIZE,
                                                        PROT_WRITE | PROT_READ,
                                                        MAP_SHARED | MAP_ANONYMOUS,
                                                        -1,
                                                        0))),
            allocator_(nullptr) {
            allocator_ = Map<K, T>::allocator_type(state_);
            if (state_ == MAP_FAILED) {
                throw MMapError{std::string("Mmap failed: ") + std::strerror(errno)};
            }

            state_->start = reinterpret_cast<char *>(state_) + sizeof(*state_);
            state_->end = reinterpret_cast<char *>(state_) + MMAP_SIZE;

            sem_ = reinterpret_cast<sem_t *>(state_->start);
            ::sem_init(sem_, 1, 1);
            state_->start += sizeof(*sem_);

            mapObject_ = reinterpret_cast<MapType *>(state_->start);

            mapObject_ = new(mapObject_) MapType{allocator_};
            state_->start += sizeof(*mapObject_);
        }

        ~Map() {
            ::munmap(state_, MMAP_SIZE);
        }

        template <typename U>
        std::enable_if_t<std::is_same_v<U, K>, T&>
        at( const U& key ) {
            SemLock lock(*sem_);
            return mapObject_->at(key);
        }

        template <typename U>
        std::enable_if_t<!std::is_same_v<U, K>, T&>
        at( const U& key ) {
            return at(utils::get_object_with_allocator_if_enable<K>(key, allocator_));
        }

        template <typename U>
        std::enable_if_t<std::is_same_v<U, K>, const T&>
        at( const U& key ) const {
            SemLock lock(*sem_);
            return mapObject_->at(key);
        }

        template <typename U>
        std::enable_if_t<!std::is_same_v<U, K>, const T&>
        at( const U& key ) const {
            return at(utils::get_object_with_allocator_if_enable<K>(key, allocator_));
        }


        template <typename U>
        std::enable_if_t<std::is_same_v<U, K>, T&>
        operator[](const U& key) {
            try {
                return at(key);
            } catch (const std::out_of_range &ex) {
                insert(std::pair<const K, T>{key, utils::get_default_object_with_allocator<T>(allocator_)});
                return at(key);
            }
        }

        template <typename U>
        std::enable_if_t<!std::is_same_v<U, K>, T&>
        operator[](const U& key) {
            return operator[](utils::get_object_with_allocator_if_enable<K>(key, allocator_));
        }

        template <typename U>
        std::enable_if_t<std::is_same_v<U, K>, size_t>
        erase(const U& key) {
            SemLock lock(*sem_);
            return mapObject_->erase(key);
        }

        template <typename U>
        std::enable_if_t<!std::is_same_v<U, K>, size_t>
        erase(const U& key) {
            return erase(utils::get_object_with_allocator_if_enable<K>(key, allocator_));
        }

    private:
        std::pair<typename MapType::iterator, bool> insert(const std::pair<const K, T>& pair_value) {
            SemLock lock(*sem_);
            return mapObject_->insert(pair_value);
        }
    };
}