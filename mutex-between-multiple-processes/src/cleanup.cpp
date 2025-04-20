#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "shared_defs.h"

const char* shm_name = "/my_shared_mutex";
const size_t shm_size = sizeof(SharedData); // replace with actual size if not using SharedData

int main() {
    // Open the shared memory (if it exists)
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open (cleanup)");
        return 1;
    }

    // Map it
    void* ptr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap (cleanup)");
        close(shm_fd);
        return 1;
    }

    // Unmap
    if (munmap(ptr, shm_size) == -1) {
        perror("munmap");
    }

    // Close the file descriptor
    if (close(shm_fd) == -1) {
        perror("close");
    }

    // Unlink the shared memory object (removes it from the system)
    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
    } else {
        std::cout << "✅ Shared memory cleaned up: " << shm_name << "\n";
    }

    return 0;
}

