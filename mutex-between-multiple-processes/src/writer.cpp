// writer.cpp
#include "shared_defs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
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

    log_with_time("Creating shared memory...");
    // Create shared memory
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        log_with_time("Failed to create shared memory.");
        return 1;
    }
    log_with_time("Shared memory created successfully.");

    log_with_time("Resizing shared memory to fit SharedData structure...");
    if (ftruncate(fd, sizeof(SharedData)) == -1) {
        log_with_time("Failed to resize shared memory.");
        close(fd);
        return 1;
    }
    log_with_time("Shared memory resized successfully.");

    log_with_time("Mapping shared memory to process address space...");
    // Map to memory
    void* ptr = mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        log_with_time("Failed to map shared memory.");
        close(fd);
        return 1;
    }
    log_with_time("Shared memory mapped successfully.");

    auto* data = reinterpret_cast<SharedData*>(ptr);

    log_with_time("Initializing inter-process mutex...");
    // Initialize mutex for inter-process
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    if (pthread_mutex_init(&data->mutex, &attr) != 0) {
        log_with_time("Failed to initialize mutex.");
        munmap(ptr, sizeof(SharedData));
        close(fd);
        return 1;
    }
    log_with_time("Mutex initialized successfully.");

    data->counter = 0;
    log_with_time("Counter initialized to 0.");

    for (int i = 0; i < 5; ++i) {
        log_with_time("Attempting to lock mutex...");
        pthread_mutex_lock(&data->mutex);
        log_with_time("Mutex locked. Incrementing counter...");
        ++data->counter;
        log_with_time("Writer incremented counter to: " + std::to_string(data->counter));
        sleep(3);
        log_with_time("Unlocking mutex...");
        pthread_mutex_unlock(&data->mutex);
        log_with_time("Mutex unlocked. Sleeping for 1 second...");
        sleep(1);
    }

    log_with_time("Cleaning up resources...");
    munmap(ptr, sizeof(SharedData));
    close(fd);
    log_with_time("Resources cleaned up. Exiting program.");

    return 0;
}
