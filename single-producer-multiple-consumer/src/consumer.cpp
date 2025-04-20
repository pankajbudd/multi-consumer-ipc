// consumer.cpp
#include "common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "[Consumer] Failed to open shared memory" << std::endl;
        return 1;
    }

    SharedMemory* shm = (SharedMemory*) mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        std::cerr << "[Consumer] Failed to map shared memory" << std::endl;
        return 1;
    }

    std::cout << "[Consumer " << getpid() << "] Attached to shared memory." << std::endl;

    int last_version = -1;

    while (true) {
        pthread_mutex_lock(&shm->mutex);
        std::cout << "[Consumer " << getpid() << "] Waiting for new version. Last seen: " << last_version << ", Current: " << shm->version << std::endl;

        while (shm->version == last_version) {
            pthread_cond_wait(&shm->cond, &shm->mutex);
        }

        int index = shm->current_index;
        std::cout << "[Consumer " << getpid() << "] Read: " << shm->buffer[index] << " (ver: " << shm->version << ")" << std::endl;
        last_version = shm->version;
        ++shm->reader_count;
        std::cout << "[Consumer " << getpid() << "] Updated reader count to " << shm->reader_count << std::endl;

        pthread_mutex_unlock(&shm->mutex);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
