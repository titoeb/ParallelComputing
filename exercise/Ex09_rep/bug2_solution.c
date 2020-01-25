/******************************************************************************
* FILE: bug2.c
* DESCRIPTION:
*   A "hello world" Pthreads program that dumps core.  Figure out why and
*   then fix it.
* AUTHOR: 9/98 Blaise Barney
* LAST REVISED: 18/01/16 Sebastian Gesemann
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 8
#define ARRAY_SIZE 1200000

void* Hello(void* threadid)
{
    double A[ARRAY_SIZE];

    long tid = (intptr_t)threadid;

    sleep(3);
    for (long i=0; i<ARRAY_SIZE; i++) {
        A[i] = i * 1.0;
    }
    printf("%ld: Hello World!   %f\n", tid, A[ARRAY_SIZE-1]);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    size_t stacksize = ARRAY_SIZE*sizeof(double) + 1024000; // about 1MB extra
    pthread_attr_setstacksize (&attr, stacksize);
    pthread_attr_getstacksize (&attr, &stacksize);
    printf("Thread stack size = %li bytes (hint, hint)\n",stacksize);
    for (intptr_t t=0; t<NTHREADS; t++){
        int rc = pthread_create(&threads[t], &attr, Hello, (void*)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    printf("Created %ld threads.\n", (long)NTHREADS);
    for (intptr_t t=0; t<NTHREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    return 0;
}

