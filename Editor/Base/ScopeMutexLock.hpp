#pragma once 

#include <mutex>

class ScopeMutexLock {
public:
    ScopeMutexLock(std::mutex& p_mutex) : mutex(p_mutex) {
        this->mutex.lock();
    }

    ~ScopeMutexLock() {
        this->mutex.unlock();
    }

private:
    std::mutex& mutex;
};