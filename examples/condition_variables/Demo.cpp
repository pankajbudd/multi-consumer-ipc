#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h> // For sleep()
#include <time.h>

// Color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

// Logger function with timestamp, thread ID and color
void log_message(const char* color, const char* message) {
    time_t now;
    struct tm* timeinfo;
    char timestamp[20];
    pthread_t tid = pthread_self();
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    printf("%s[%s][Thread-%lu] %s%s", color, timestamp, (unsigned long)tid, message, RESET);
}

/* For safe condition variable usage, must use a boolean predicate and  */
/* a mutex with the condition.                                          */
int                 workToDo = 0;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

#define NTHREADS      5

void *threadfunc(void *parm)
{
  int           rc;

  while (1) {
    /* Usually worker threads will loop on these operations */
    rc = pthread_mutex_lock(&mutex);
    log_message(BLUE, "pthread_mutex_lock()\n");

    while (!workToDo) {
      log_message(YELLOW, "Thread blocked\n");
      rc = pthread_cond_wait(&cond, &mutex);
      log_message(GREEN, "pthread_cond_wait()\n");
    }
    log_message(GREEN, "Thread awake, finish work!\n");

    /* Under protection of the lock, complete or remove the work     */
    /* from whatever worker queue we have. Here it is simply a flag  */
    workToDo = 0;

    rc = pthread_mutex_unlock(&mutex);
    log_message(BLUE, "pthread_mutex_unlock()\n");
  }
  return NULL;
}

int main(int argc, char **argv)
{
  int                   rc=0;
  int                   i;
  pthread_t             threadid[NTHREADS];

  log_message(GREEN, "Enter Testcase - main thread started\n");

  log_message(BLUE, "Creating threads...\n");
  for(i=0; i<NTHREADS; ++i) {
    rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
    log_message(GREEN, "pthread_create()\n");
  }

  sleep(5);  /* Sleep is not a very robust way to serialize threads   */

  for(i=0; i<2; ++i) {
    log_message(YELLOW, "Wake up a worker, work to do...\n");

    rc = pthread_mutex_lock(&mutex);
    log_message(BLUE, "pthread_mutex_lock()\n");

    /* In the real world, all the threads might be busy, and        */
    /* we would add work to a queue instead of simply using a flag  */
    /* In that case the boolean predicate might be some boolean     */
    /* statement like: if (the-queue-contains-work)                 */
    if (workToDo) {
       log_message(RED, "Work already present, likely threads are busy\n");
    }
    workToDo = 1;
    //rc = pthread_cond_signal(&cond);

	// Used thread_cond_signal(&cond) here to check behavior of progr
	rc = pthread_cond_broadcast(&cond);
    sleep(2);  /* Sleep is not a very robust way to serialize threads */

    log_message(GREEN, "pthread_cond_signal()\n");

    rc = pthread_mutex_unlock(&mutex);
    log_message(BLUE, "pthread_mutex_unlock()\n");
    sleep(5);  /* Sleep is not a very robust way to serialize threads */
  }

  log_message(GREEN, "Main completed\n");
  //exit(0);
  return 0;
}
