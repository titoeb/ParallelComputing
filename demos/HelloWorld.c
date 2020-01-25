#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>


/* Global Variable can be changed by all threads! */
int thread_count;

/* Thread function */

void *Hello(void *rank){
	long my_rank = (long) rank;
	printf("Hello from thread %ld of %d\n", my_rank, thread_count);

	return NULL;
}

int main(int argc, char *argv[]) {
	long thread;
	pthread_t *thread_handles;

	/* Get the number of threads from the command line */
	thread_count = atoi(argv[1]);
	
	/* create handle array */
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++){
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}
	printf("Hello from the main thread\n");

	for (thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
	}

	free(thread_handles);
	return 0;
}	
	



