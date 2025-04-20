// common.h
#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

#define SHM_NAME "/shm_flipflop"
#define BUFFER_SIZE 2
#define STRING_SIZE 11  // 10 chars + null terminator
#define MAX_CONSUMERS 2

struct SharedMemory {
    char buffer[BUFFER_SIZE][STRING_SIZE];
    int current_index;   // 0 or 1: flip-flop
    int reader_count;    // Number of consumers that have read the current data
    int active_consumers;// Number of consumers currently running
    int version;         // Version incremented per write

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

#endif