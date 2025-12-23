#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>
#include "shared_defs.h"

const char* shm_name = "my_shared_mutex";

int main() {
    // Remove the shared memory object
    if (boost::interprocess::shared_memory_object::remove(shm_name)) {
        std::cout << "✅ Shared memory cleaned up: " << shm_name << "\n";
    } else {
        std::cout << "❌ Failed to remove shared memory: " << shm_name << "\n";
    }

    return 0;
}

