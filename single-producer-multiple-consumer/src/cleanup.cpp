#include "common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

int main() {
    shm_unlink(SHM_NAME);
    std::cout << "Shared memory cleaned up.\n";
    return 0;
}
