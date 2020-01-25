#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<float.h>
#include<limits.h>
#include<math.h>
#include<time.h>


// Define mutex
pthread_mutex_t lock;

// Define a struct for the paramtes of the function
struct par_trap_args{
	int thread_id;
	double start;
	double end;
	int steps;
	double(*fun)(double);
	double *result;
};

double f(double x){
	return sin(x);
}

double trap_seq(double start, double end, int steps, double (*fun)(double)){
	double apprx = 0.0;
	double interval = (end - start) / steps;

	for(double val = start; val <= end; val = val + interval){
		apprx += ((fun(val) + fun(val + interval)) / 2) * interval;
	}
	return apprx;
}

void *par_trap(void *args){
	struct par_trap_args *arg = (struct par_trap_args*) args;
	
	int thread_id = arg->thread_id;
	double start = arg->start;
	double end = arg->end;
	int steps = arg->steps;
	double(*fun)(double) = arg->fun;
	double *result = arg->result;
	//printf("Thread %d: From %lf to %lf with %d steps\n", thread_id, start, end, steps);	

	double apprx = 0.0;
	double interval = (end - start) / steps;

	for(double val = start; val <= end; val = val + interval){
		apprx += ((fun(val) + fun(val + interval)) / 2) * interval;
	}
	
	pthread_mutex_lock(&lock);
	*result += apprx;
	pthread_mutex_unlock(&lock);
}
double trap_par(double start, double end, int steps, double (*fun)(double), int threads){
	double apprx = 0.0;
	pthread_t all_thread[threads];

	/* intiallize lock */
	 if (pthread_mutex_init(&lock, NULL) != 0)
	 {
	 	//printf("\n mutex init failed\n");
	 	return 1;
         }

	/* create pointer to array of struct par_trap_args to store the arguments of each thread function */
	struct par_trap_args all_args[threads];

	/* save start, end and steps in an array on heap */

	/* start all threads and give them their interval */
	double step = (end - start) / threads;
	for(int i = 0; i < threads; i++){
		/* calculate args for each thread */
		all_args[i].thread_id = i;
		all_args[i].start = 0.0 + i * step;
		all_args[i].end= (i + 1) * step;
		all_args[i].steps = steps / threads;
		all_args[i].fun = fun;
		all_args[i].result = &apprx;
		
		/* start thread */
		pthread_create(&all_thread[i], NULL, par_trap, (void *)&all_args[i]);
	}
	
	for(int i = 0; i < threads; i++){
		pthread_join(all_thread[i], NULL);
	}
	
	return apprx;
}

int main(int argc, char *argv[]){
	double start = 0;
	double end = 100000;
	int steps = 2000000000;
	/*double end = 8;
	int steps = 1000;  */
	double (*fun)(double) = &f;
	double res_seq;
	double time_spent;
	struct timespec begin, end_t;

	clock_gettime(CLOCK_REALTIME, &begin);
	res_seq = trap_seq(start, end,  steps, fun);
	clock_gettime(CLOCK_REALTIME, &end_t);
	time_spent = (end_t.tv_sec - begin.tv_sec) + (end_t.tv_nsec - begin.tv_nsec) / 1000000000.0;

	printf("Sequential. From %lf to %lf with %d steps is %lf and took %lf seconds.\n", start, end, steps, res_seq, time_spent);

	clock_gettime(CLOCK_REALTIME, &begin);
	res_seq = trap_par(start, end,  steps, fun, atoi(argv[1]));
	clock_gettime(CLOCK_REALTIME, &end_t);
	time_spent = (end_t.tv_sec - begin.tv_sec) + (end_t.tv_nsec - begin.tv_nsec) / 1000000000.0;

	printf("Parallel. From %lf to %lf with %d steps is %lf and took %lf seconds.\n", start, end, steps, res_seq, time_spent);

}









