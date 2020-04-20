#ifndef SHMEM_SEMLOCK_HPP
#define SHMEM_SEMLOCK_HPP

#include <semaphore.h>

namespace utils {
    class SemLock {
    public:
        explicit SemLock(sem_t &sem);
        ~SemLock();

    private:
        sem_t &sem_;
    };
}

#endif //ADVCPPTP_SEMLOCK_HPP
