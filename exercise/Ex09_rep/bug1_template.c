/******************************************************************************
* FILE: bug1.c
* DESCRIPTION:
*   This example has a bug. It is a variation on the condvar.c example. 
*   Instead of just one thread waiting for the condition signal, there are
*   four threads waiting for the same signal. Find out how to fix the
*   program.
* SOURCE: Adapted from example code in "Pthreads Programming", B. Nichols
*   et al. O'Reilly and Associates.
* LAST REVISED: 01/18/16  Sebastian Gesemann
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>  // for sleep

#define NUM_THREADS 6
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void* inc_count(void *idp) 
{
    long my_id = (long)idp;
    for (int i=0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;
        // Check the value of count and signal waiting thread when condition is
        // reached.  Note that this occurs while mutex is locked. 
        if (count == COUNT_LIMIT) {
            pthread_cond_signal(&count_threshold_cv);
            printf("inc_count(): thread %ld, count = %d  Threshold reached.\n", my_id, count);
        }
        printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
        pthread_mutex_unlock(&count_mutex);

        // Do some work so threads can alternate on mutex lock
        sleep(1);
    }
    return NULL;
}

void* watch_count(void *idp)
{
    long my_id = (long)idp;

    printf("Starting watch_count(): thread %ld\n", my_id);

    // Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
    // will automatically and atomically unlock mutex while it waits. 
    // Also, note that if COUNT_LIMIT is reached before this routine is run by
    // the waiting thread, the loop will be skipped to prevent pthread_cond_wait
    // from never returning.
    pthread_mutex_lock(&count_mutex);
    printf("***Before cond_wait: thread %ld\n", my_id);
    pthread_cond_broadcast(&count_threshold_cv);
    printf("***Thread %ld Condition signal received.\n", my_id);
    pthread_mutex_unlock(&count_mutex);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];

    // Initialize mutex and condition variable objects
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    // start threads, NUM_THREADS-2 watchers and 2 incrementing the counter
    for (intptr_t i = 2; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, watch_count, (void*)i);
    }
    pthread_create(&threads[0], NULL, inc_count, (void*)0);
    pthread_create(&threads[1], NULL, inc_count, (void*)1);

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

    // Clean up and exit
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    return 0;
}

