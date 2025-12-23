// writer.cpp
#include "shared_defs.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
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
    const char* shm_name = "my_shared_mutex";

    log_with_time("Creating shared memory...");
    // Create shared memory
    boost::interprocess::shared_memory_object shm(boost::interprocess::create_only, shm_name, boost::interprocess::read_write);
    shm.truncate(sizeof(SharedData));
    log_with_time("Shared memory created and resized successfully.");

    log_with_time("Mapping shared memory to process address space...");
    // Map to memory
    boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
    void* ptr = region.get_address();
    log_with_time("Shared memory mapped successfully.");

    auto* data = reinterpret_cast<SharedData*>(ptr);

    // Construct the mutex and condition in shared memory
    new (&data->mutex) boost::interprocess::interprocess_mutex();
    new (&data->cond) boost::interprocess::interprocess_condition();

    data->counter = 0;
    log_with_time("Counter initialized to 0.");

    for (int i = 0; i < 5; ++i) {
        log_with_time("Attempting to lock mutex...");
        data->mutex.lock();
        log_with_time("Mutex locked. Incrementing counter...");
        ++data->counter;
        log_with_time("Writer incremented counter to: " + std::to_string(data->counter));
        sleep(3);
        log_with_time("Unlocking mutex...");
        data->mutex.unlock();
        log_with_time("Mutex unlocked. Sleeping for 1 second...");
        sleep(1);
    }

    log_with_time("Cleaning up resources...");
    // Shared memory will be removed by cleanup program
    log_with_time("Resources cleaned up. Exiting program.");

    return 0;
}
