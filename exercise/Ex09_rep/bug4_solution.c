/******************************************************************************
* FILE: bug4.c
* DESCRIPTION:
*   This program demonstrates a condition variable race/synchronization 
*   problem. It resembles the condvar.c program.
* SOURCE: 07/06/05 Blaise Barney
* LAST REVISED: 01/18/16  Sebastian Gesemann
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define ITERATIONS 10
#define THRESHOLD 12

int count = 0;
double finalresult = 0.0;
pthread_mutex_t count_mutex;
pthread_cond_t count_condvar;

void* sub1(void* t)
{
    long tid = (intptr_t)t;
 
    // do some work
    sleep(1);

    // Lock mutex and wait for the condition "count>=THRESHOLD"
    // to be satisfied...
    pthread_mutex_lock(&count_mutex);
    while (count < THRESHOLD) {
        printf("sub1: thread=%ld going into wait. count=%d\n", tid, count);
        pthread_cond_wait(&count_condvar, &count_mutex);
        printf("sub1: thread=%ld Condition variable signal received.\n", tid);
    }
    printf("sub1: thread=%ld, count=%d, finalresult=%f\n",tid,count,finalresult);
    pthread_mutex_unlock(&count_mutex);

    return NULL;
}

void* sub2(void* t) 
{
    long tid = (intptr_t)t;
    for (int i=0; i<ITERATIONS; i++) {
        double myresult = 0.0;
        for (int j=0; j<100000; j++) {
            myresult += sin(j) * tan(i);
        }
        pthread_mutex_lock(&count_mutex);
        finalresult += myresult;
        count++;
        // Check the value of count and signal waiting thread when condition is
        // reached.  Note that this occurs while mutex is locked. 
        if (count == THRESHOLD) {
            printf("sub2: thread=%ld Threshold reached. count=%d. ",tid,count);
            pthread_cond_signal(&count_condvar);
            printf("      Just sent signal.\n");
        } else {
            printf("sub2: thread=%ld did work. count=%d\n",tid,count);
        }
        pthread_mutex_unlock(&count_mutex);
    }
    printf("sub2: thread=%ld Done.\n",tid);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];

    // Initialize mutex and condition variable objects
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_condvar, NULL);

    pthread_create(&threads[0], NULL, sub1, (void*)0);
    for (intptr_t i = 1; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, sub2, (void*)i);
    }

    // Wait for all threads to complete
    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Main(): Waited on %d threads. Final result=%e. Done.\n",
           NUM_THREADS,finalresult);

    // Clean up and exit
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_condvar);
    return 0;
}

