/*****************************************************************************
* FILE: hello_arg3.c
* DESCRIPTION:
*   This "hello world" Pthreads program demonstrates an unsafe and incorrect
*   way to pass thread arguments at thread creation.
* AUTHOR: Blaise Barney
* LAST REVISED: 01/08/16 Sebastian Gesemann
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>   // for sleep(unsigned) taking a duration in seconds
#include <pthread.h>

#define NUM_THREADS 8

void* PrintHello(void* arg)
{
   sleep(1);
   int thread_id_ptr = (intptr_t) arg;
   printf("Hello from thread %d\n", thread_id_ptr);
   return NULL;
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];

   for(int t = 0; t < NUM_THREADS; t++) {
      printf("Creating thread %d\n", t);
      int error = pthread_create(&threads[t], NULL, PrintHello,  (void*) t);
      if (error) {
         printf("ERROR; return code from pthread_create() is %d\n", error);
         exit(-1);
      }
   }

   sleep(2);
 
   return 0;
}

