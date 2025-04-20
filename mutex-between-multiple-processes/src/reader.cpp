// reader.cpp
#include "shared_defs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <iomanip>

void log_with_time(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::cout << "[" << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
              << "." << std::setfill('0') << std::setw(3) << ms.count() << "] " << message << "\n";
}

int main() {
    const char* shm_name = "/my_shared_mutex";

    log_with_time("Opening shared memory: " + std::string(shm_name));
    int fd = shm_open(shm_name, O_RDWR, 0666);
    if (fd == -1) {
        log_with_time("Failed to open shared memory. Exiting.");
        return 1;
    }

    log_with_time("Mapping shared memory...");
    void* ptr = mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        log_with_time("Failed to map shared memory. Exiting.");
        close(fd);
        return 1;
    }

    auto* data = reinterpret_cast<SharedData*>(ptr);
    log_with_time("Shared memory mapped successfully.");

    for (int i = 0; i < 5; ++i) {
        log_with_time("Attempting to lock mutex...");
        pthread_mutex_lock(&data->mutex);
        log_with_time("Mutex locked. Reader sees counter: " + std::to_string(data->counter));
        sleep(5);
        log_with_time("Unlocking mutex...");
        pthread_mutex_unlock(&data->mutex);
        log_with_time("Mutex unlocked. Sleeping for 1 second before next iteration.");
        sleep(1);
    }

    log_with_time("Unmapping shared memory...");
    munmap(ptr, sizeof(SharedData));
    log_with_time("Closing shared memory file descriptor...");
    close(fd);
    log_with_time("Reader process completed.");

    return 0;
}
