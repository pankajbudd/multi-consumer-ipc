// reader.cpp
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

    log_with_time("Opening shared memory: " + std::string(shm_name));
    boost::interprocess::shared_memory_object shm(boost::interprocess::open_only, shm_name, boost::interprocess::read_write);

    log_with_time("Mapping shared memory...");
    boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
    void* ptr = region.get_address();

    auto* data = reinterpret_cast<SharedData*>(ptr);
    log_with_time("Shared memory mapped successfully. Data pointer: " + std::to_string(reinterpret_cast<uintptr_t>(data)));

    for (int i = 0; i < 5; ++i) {
        log_with_time("Attempting to lock mutex... (iteration " + std::to_string(i) + ")");
        log_with_time("Mutex address: " + std::to_string(reinterpret_cast<uintptr_t>(&data->mutex)));
        data->mutex.lock();
        log_with_time("Mutex locked. Reader sees counter: " + std::to_string(data->counter));
        sleep(5);
        log_with_time("Unlocking mutex...");
        data->mutex.unlock();
        log_with_time("Mutex unlocked. Sleeping for 1 second before next iteration.");
        sleep(1);
    }

    log_with_time("Reader process completed.");

    return 0;
}
