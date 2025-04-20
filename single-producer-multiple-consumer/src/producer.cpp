// producer.cpp
#include "common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <thread>

void random_string(char *str, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length - 1; ++i)
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    str[length - 1] = '\0';
}

int main() {
    srand(time(nullptr));

    std::cout << "[Producer] Starting up and creating shared memory." << std::endl;
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "[Producer] Error: Failed to create shared memory." << std::endl;
        return 1;
    }

    ftruncate(shm_fd, sizeof(SharedMemory));
    SharedMemory* shm = (SharedMemory*) mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        std::cerr << "[Producer] Error: Failed to map shared memory." << std::endl;
        return 1;
    }

    // Initialize only once
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mutex, &mattr);

    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shm->cond, &cattr);

    shm->current_index = 0;
    shm->reader_count = 0;
    shm->version = 0;
    shm->active_consumers = MAX_CONSUMERS;

    std::cout << "[Producer] Initialized shared memory and synchronization primitives." << std::endl;

    while (true) {
        pthread_mutex_lock(&shm->mutex);

        std::cout << "[Producer] Locked mutex. Reader count: " << shm->reader_count
                  << ", Active consumers: " << shm->active_consumers
                  << ", Current index: " << shm->current_index
                  << ", Version: " << shm->version << std::endl;

        // Wait for all consumers to read
        if (shm->reader_count >= shm->active_consumers) {
            shm->current_index = 1 - shm->current_index;
            shm->reader_count = 0;
            std::cout << "[Producer] All consumers read. Flipping buffer index to " << shm->current_index << std::endl;
        }

        // Write random string
        int index = shm->current_index;
        random_string(shm->buffer[index], STRING_SIZE);
        ++shm->version;
        std::cout << "[Producer] Wrote: " << shm->buffer[index] << " to buffer index: " << index << " (version: " << shm->version << ")" << std::endl;

        pthread_cond_broadcast(&shm->cond);
        std::cout << "[Producer] Broadcasted condition to consumers." << std::endl;
        pthread_mutex_unlock(&shm->mutex);
        std::cout << "[Producer] Unlocked mutex after write." << std::endl;

        // If consumers have not read, wait; else write again
        while (true) {
            pthread_mutex_lock(&shm->mutex);
            std::cout << "[Producer] Checking if consumers read... Reader count: " << shm->reader_count << ", Expected: " << shm->active_consumers << std::endl;
            if (shm->reader_count >= shm->active_consumers) {
                std::cout << "[Producer] All consumers have read the buffer. Proceeding to next write." << std::endl;
                pthread_mutex_unlock(&shm->mutex);
                break;
            }
            pthread_mutex_unlock(&shm->mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // prevent tight loop
        }
    }

    return 0;
}
