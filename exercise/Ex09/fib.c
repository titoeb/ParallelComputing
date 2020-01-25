#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
	long double tmp;
	long double curr = 1.0;
	long double last = 1.0;
	for(int i = 3; i < 100; i++){
		tmp = curr;
		curr = curr + last;
		last = tmp;
		printf("The %d's fibonacci number is %Lf\n", i, curr);
	}
	printf("\n Now in parallel!\n \n");
	
	curr = 1.0;
	last = 1.0;
	
	#pragma omp parallel for num_threads(3)	
	for(int i = 3; i < 100; i++){
		tmp = curr;
		curr = curr + last;
		last = tmp;
		printf("The %d's fibonacci number is %Lf\n", i, curr);
	}

}
