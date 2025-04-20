// shared_defs.h
#include <pthread.h>

struct SharedData {
    pthread_mutex_t mutex;
    int counter;
};

