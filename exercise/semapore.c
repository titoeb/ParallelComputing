#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>  // for sleep


sem_t club_entrance;

void *thread_work(void *input){
	int thread_id = (int) input;
	
	
	sem_wait(&club_entrance);
	printf("Thread %d entered the Club!\n", thread_id);
	sleep(1);
	sem_post(&club_entrance);	

	printf("Thread %d left the Club!\n", thread_id);
}


int main(){
	pthread_t threads[20];
	sem_init(&club_entrance, 0, 3);

	 for(int i = 0; i < 20; i++)
    		pthread_create(&threads[i], NULL, thread_work, (void*)i);

  	for(int i = 0; i < 20; i++)
   		pthread_join(threads[i], NULL);

	sem_destroy(&club_entrance);
	return 0;
}
