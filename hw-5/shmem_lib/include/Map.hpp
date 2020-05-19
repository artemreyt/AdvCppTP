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

    const size_t MMAP_DEFAULT_SIZE = 8192;

    template <typename K, typename T>
    class Map {
        using allocator_type = Allocator<std::pair<const K, T>>;
        using MapType = std::map<K, T, std::less<K>, allocator_type>;

        MapType         *mapObject_;
        allocator_type  allocator_;
        AllocState      *state_;
        sem_t           *sem_;

    public:

        Map(size_t mmap_size=MMAP_DEFAULT_SIZE): allocator_(nullptr) {

            auto mmap = ::mmap(nullptr, mmap_size, PROT_WRITE | PROT_READ,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            if (mmap == MAP_FAILED) {
                throw MMapError{std::string("Mmap failed: ") + std::strerror(errno)};
            }

            state_ = static_cast<AllocState *>(mmap);
            allocator_ = allocator_type(state_);

            state_->start = reinterpret_cast<char *>(state_) + sizeof(*state_);
            state_->end = reinterpret_cast<char *>(state_) + mmap_size;

            sem_ = reinterpret_cast<sem_t *>(state_->start);
            if (::sem_init(sem_, 1, 1) == -1) {
                ::munmap(state_, MMAP_DEFAULT_SIZE);
                throw MMapError{std::string("Semaphore init failure: ") + std::strerror((errno))};
            }

            state_->start += sizeof(*sem_);
            mapObject_ = reinterpret_cast<MapType *>(state_->start);
            mapObject_ = new(mapObject_) MapType{allocator_};
            state_->start += sizeof(*mapObject_);
        }

        ~Map() {
            ::munmap(state_, MMAP_DEFAULT_SIZE);
        }

        T& at( const K& key ) {
            SemLock lock(*sem_);
            return mapObject_->at(key);
        }

        template <typename U>
        T& at( const U& key ) {
            return at(utils::get_object_with_allocator<K>(key, allocator_));
        }

        size_t erase(const K& key) {
            SemLock lock(*sem_);
            return mapObject_->erase(key);
        }

        template <typename U>
        size_t erase(const U& key) {
            return erase(utils::get_object_with_allocator<K>(key, allocator_));
        }

        const allocator_type& get_allocator() const {
            return allocator_;
        }

        auto insert(const K &key, const T &value) {
            SemLock lock(*sem_);
            return mapObject_->insert_or_assign(key, value);
        }

        template<typename KeyType, typename ValueType>
        auto insert(const KeyType &key_, const ValueType &value_) {
            auto key = utils::get_object_with_allocator<K>(key_, allocator_);
            auto value = utils::get_object_with_allocator<T>(value_, allocator_);

            SemLock lock(*sem_);
            return mapObject_->insert_or_assign(std::move(key), std::move(value));
        }
    };
}