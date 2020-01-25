/*****************************************************************************
* FILE: bug6.c
* DESCRIPTION:
*   This example demonstrates a data race with a global variable that
*   gives obviously wrong results. Figure out how to fix the problem.
* SOURCE: 07/06/05 Blaise Barney
* LAST REVISED: 01/15/16 Sebastian Gesemann
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

/* Define global data where everyone can see them */
#define NUMTHRDS 8
#define VECLEN 100000
int* a;
int* b;
long sum = 0;
int global_flag = 0;

void* dotprod(void* arg)
{
   // Each thread works on a different set of data.
   // The offset is specified by the arg parameter. The size of
   // the data for each thread is indicated by VECLEN.
   long tid   = (intptr_t) arg; // [0, NUMTHRDS)
   long len   = VECLEN;
   long start = tid * len;
   long end   = start + len;
   long my_sum = 0;

   // Perform my section of the dot product
   printf("thread: %ld starting. start=%ld end=%ld (exclusive)\n", tid, start, end);
   for (long i = start; i < end ; i++) 
      my_sum += a[i] * b[i];
   printf("thread: %ld done. Global sum is now %ld\n", tid, sum);
   while(global_flag != tid);
   sum += my_sum;
   global_flag++;
	
   return NULL;
}

int main(int argc, char *argv[])
{

   // Assign storage and initialize values
   a = malloc(sizeof(int) * NUMTHRDS * VECLEN);
   b = malloc(sizeof(int) * NUMTHRDS * VECLEN);

   for (long i = 0; i < VECLEN * NUMTHRDS; i++)
      a[i] = b[i] = 1;

   // Create threads as joinable, each of which will execute the dot product
   // routine. Their offset into the global vectors is specified by passing
   // the "i" argument in pthread_create().
   pthread_t threads[NUMTHRDS];
   for (intptr_t i = 0; i < NUMTHRDS; i++) 
      pthread_create(&threads[i], NULL, dotprod, (void*) i);

   // Wait on the threads for final result */
   for(int i = 0; i < NUMTHRDS; i++)
      pthread_join(threads[i], NULL);

   // After joining, print out the results and cleanup
   printf("Final Global Sum = %ld\n", sum);
   free(a);
   free(b);

   return 0;
}   

