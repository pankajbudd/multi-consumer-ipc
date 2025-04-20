#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Shared resources
std::mutex mtx;
/**
 * @brief Used to synchronize threads by allowing one or more threads to wait 
 *        until they are notified by another thread.
 * 
 * @details The std::condition_variable is a synchronization primitive that can 
 *          be used to block a thread, or multiple threads, until another thread 
 *          modifies a shared variable (typically protected by a std::mutex) and 
 *          notifies the condition_variable. It is commonly used in producer-consumer 
 *          scenarios or other multi-threaded applications where thread coordination 
 *          is required.
 * 
 * @note Always use std::unique_lock<std::mutex> when waiting on a condition_variable.
 *       The associated mutex must be locked before calling wait(), and it will be 
 *       automatically released while waiting and re-acquired after being notified.
 * 
 * @see std::mutex, std::unique_lock
 */
std::condition_variable cv;
bool dataReady = false;

// Producer function
void producer() {
    // Simulate work
    std::cout << "Producer: Starting up\n";
    std::cout << "Producer: Sleeping for 1 second\n";
    /**
     * @brief Summary Table:
     * 
     * | Feature              | std::this_thread::sleep_for | sleep()         | usleep()        |
     * |----------------------|-----------------------------|-----------------|-----------------|
     * | Precision            | High (chrono-based)        | Seconds only    | Microseconds    |
     * | Platform Support     | Cross-platform             | POSIX only      | POSIX only      |
     * | Header               | <thread>                   | <unistd.h>      | <unistd.h>      |
     * | Time Units           | Type-safe (chrono::)       | int (seconds)   | int (microseconds) |
     * | Deprecation Status   | ✅ Modern and recommended  | ❌ Limited precision | ⚠️ Deprecated |
     * | Can sleep < 1 second | ✔️                         | ❌              | ✔️              |
     */
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        /**
         * @brief Ensures thread-safe access to shared resources by locking the mutex.
         * 
         * The std::lock_guard is used here to manage the mutex `mtx` in a scoped manner.
         * It automatically locks the mutex when the lock_guard object is created and 
         * unlocks it when the lock_guard goes out of scope. This prevents potential 
         * deadlocks or race conditions by ensuring that the mutex is properly released 
         * even if an exception is thrown within the scope.
         */
        std::lock_guard<std::mutex> lock(mtx);
        dataReady = true;
        std::cout << "Producer: Data ready, waiting 2 seconds\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate additional work
        std::cout << "Producer: Waiting 1 more second\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate additional work

    // If notify_one is used, only one thread will be notified.
    // cv.notify_one(); // Notify one waiting thread, which will acquire the lock first (e.g., consumer1, not consumer2 or consumer3).
    // If notify_all is used, all waiting threads will be notified (e.g., consumer1, consumer2, and consumer3).
    cv.notify_all(); // Notify all waiting threads.

}

// Consumer 1 function
void consumer1() {
    /**
     * @brief Acquires a unique lock on the given mutex.
     * 
     * This code uses `std::unique_lock` to manage the locking and unlocking
     * of the mutex `mtx`. The `std::unique_lock` is preferred here because:
     * - It provides more flexibility compared to `std::lock_guard`, such as
     *   the ability to lock and unlock the mutex multiple times within its scope.
     * - It ensures exception safety by automatically releasing the lock when
     *   the `std::unique_lock` object goes out of scope, preventing deadlocks.
     * 
     * This approach is particularly useful in scenarios where the lock needs
     * to be conditionally released or transferred.
     */
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return dataReady; }); // Wait until dataReady is true
    std::cout << "Consumer1: Got the data\n";
}

// Consumer 2 function
void consumer2() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return dataReady; }); // Wait until dataReady is true
    std::cout << "Consumer2: Got the data\n";
}

// Consumer 3 function
void consumer3() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return dataReady; }); // Wait until dataReady is true
    std::cout << "Consumer3: Got the data\n";
}

// Main function
int main() {
    // Create threads
    std::thread t1(producer);
    std::thread t2(consumer1);
    std::thread t3(consumer2);

    // Join threads
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
