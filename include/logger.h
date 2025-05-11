#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <pthread.h>

// Color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

// Logger function with timestamp, thread ID and color
inline void log_message(const char* color, const char* message) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::cout << color
              << "[" << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
              << '.' << std::setfill('0') << std::setw(3) << now_ms.count() << "]"
              << "[Thread-" << (unsigned long)pthread_self() << "] " 
              << message << RESET << std::endl;
}

#endif // LOGGER_H 