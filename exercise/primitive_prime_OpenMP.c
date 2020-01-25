#include<stdio.h>
#include<stdlib.h>
# include <omp.h>
#include<float.h>
#include<limits.h>
#include<math.h>
#include<time.h>

int is_prime_par(long long cand, int threads){
	if((cand % 2 == 0) & (cand != 2)) return 0;
	else if(cand == 2) return 1;
	
	long long squ = (long long) sqrt( (double) cand);

	#pragma omp parallel for num_threads(threads)
	for(long long i = 3; i < squ + 1; i+=2){
		if(cand % i == 0) return 0;
	}
	return 1;


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


















