#include "SemLock.hpp"

namespace utils {
    SemLock::SemLock(sem_t&sem) :
        sem_(sem){
        ::sem_wait(&sem_);
    }

    SemLock::~SemLock() {
        ::sem_post(&sem_);
    }
}

