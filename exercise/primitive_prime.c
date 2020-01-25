#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<float.h>
#include<limits.h>
#include<math.h>
#include<time.h>


// Mutex used for crucial commmunication between threads (when to abort execution)
pthread_mutex_t lock;

// a pointer to on instance of the struct will be send to each thread to get its paramters.
struct par_is_prime_args {
	int thread_id;
	long long from;
	long long to;
	long long candidate;
	int *execute;
};


// This is the function that each thread has to execute. args will contain a pointer to an par_is_prime_args struct 
void *par_is_prime(void *args){
	struct par_is_prime_args *arg = (struct par_is_prime_args*) args;
	
	// The value for start may be 0. If this is the case start from 3 otherwise a floating-point exception may oocur.
	long long start = 3;	
	if(arg->from >3) start = arg->from;
	
	for(long long i = start; (i <= arg->to) & (*(arg->execute) == 1); i+=2){
		if((arg->candidate  % i == 0) & (arg->candidate != i)){
			// Now we have to signal all threads via art-> execute that we found the variable!
			//printf("Thread %d detected: %lld is divisible by %lld\n", arg->thread_id, arg->candidate, i);
			pthread_mutex_lock(&lock);
			*(arg->execute) = 0;
			pthread_mutex_unlock(&lock);	
		}
	}
}

int is_prime_par(long long cand, int threads){
	// Check whether input is either divisible by two or two. If one of these return either prime or not accordingly.
	if((cand % 2 == 0) & (cand != 2)) return 0;
	else if(cand == 2) return 1;

	// We have to check for all number 3, ... , square_root(cand)
	long long squ = (long long) sqrt( (double) cand);

	// Here, all thread_ID are save
	pthread_t all_thread[threads];
	
	// Here all parameter structs are saved. One for each thread that contain its start and finish value;
	struct par_is_prime_args all_args[threads];

	// This variable will be checked by all threads whether one found out that cand was not a prime.
	int signal_finish = 1;

	// This is the effective number of number (/ 2) that each thread has to check the input for
	long long inter = squ / threads;

	// Make sure that this number is not even. Otherwise some of the threads might look only for number divisible by two! 
	if(inter % 2 == 0) inter++;

	for (int i = 0; i < threads; i++){
		all_args[i].from = i * inter;
		all_args[i].to = i * inter + inter;
		all_args[i].candidate = cand;
		all_args[i].execute = &signal_finish;
		all_args[i].thread_id = i;
		//printf("Thread %d from: %lld, to: %lld\n", i, all_args[i].from, all_args[i].to); // debug
		pthread_create(&all_thread[i], NULL, par_is_prime, (void *)&all_args[i]);
	}

	for(int i = 0; i < threads; i++){
		pthread_join(all_thread[i], NULL);
	}
	

	return signal_finish;
}


int is_prime_ser(long long cand){
	if((cand % 2 == 0) & (cand != 2)) return 0;
	else if(cand == 2) return 1;
	
	long long squ = (long long) sqrt( (double) cand);
	
	for(long long i = 3; i < squ + 1; i+=2){
		if(cand % i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]){
	long long candidate = 100000;

	double time_spent;
	struct timespec begin, end_t;

	clock_gettime(CLOCK_REALTIME, &begin);
	for(long long i = 2; i < candidate; i++){
		is_prime_ser(i);
	}
	
	clock_gettime(CLOCK_REALTIME, &end_t);
	time_spent = (end_t.tv_sec - begin.tv_sec) + (end_t.tv_nsec - begin.tv_nsec) / 1000000000.0;

	printf("Sequential. Checking all numbers below %lld for primes took %lf seconds.\n", candidate, time_spent);
	
	int num =  atoi(argv[1]);
	clock_gettime(CLOCK_REALTIME, &begin);
	for(long long i = 2; i < candidate; i++){
		is_prime_par(i, num);
	}

	clock_gettime(CLOCK_REALTIME, &end_t);
	time_spent = (end_t.tv_sec - begin.tv_sec) + (end_t.tv_nsec - begin.tv_nsec) / 1000000000.0;

	printf("Parallel. Checking all numbers below %lld for primes took %lf seconds.\n", candidate, time_spent);
}


















