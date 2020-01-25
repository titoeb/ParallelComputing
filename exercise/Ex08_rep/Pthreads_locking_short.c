#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 1000
#define CRITICAL_SECTIONS 100
#define SOME_WORK 2

int data = 0; /* global data variable */

pthread_mutex_t mutex;

void* mythread(void* threadid)
{
  int i;
  for(i=0; i<CRITICAL_SECTIONS; i++)
  {
    int tmpdata = data;

    long ws;
    for(ws=0; ws<1<<SOME_WORK; ws++) { int j = ws*i + j*i; }

    data = tmpdata + 1;
  }
  pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
  pthread_t *threads;
  long t;

  pthread_mutex_init(&mutex, NULL);
  threads = (pthread_t*) calloc(NUM_THREADS, (size_t)sizeof(pthread_t));

  for(t=0; t<NUM_THREADS; t++)
    pthread_create(&threads[t], NULL, mythread, (void*)t);

  for(t=0; t<NUM_THREADS; t++)
    pthread_join(threads[t], NULL);

  printf("data = %d (%d)\n", data, NUM_THREADS*CRITICAL_SECTIONS);

  pthread_mutex_destroy(&mutex);
  pthread_exit(NULL);
}
