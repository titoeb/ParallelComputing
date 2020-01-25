#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char* argv[]){
	
	MPI_Init(&argc, &argv);

	int my_rank, my_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &my_size);

	if(my_rank == 0){
		// Main process

		// Read in data from 
		int data[argc - 1];
		
		printf("This is process 0 and I read in the following data: ");		
		for(int i = 0; i < argc - 1; i++){
			data[i] = atoi(argv[i + 1]);
			printf("%d ", data[i]);
		}
		printf("\n");
		
		// Communicate the size of the data to all other working processes
		MPI_Bcast(&argc, 1, MPI_INT, 0, MPI_COMM_WORLD);

		// broadcast data itself
		MPI_Bcast(&data, argc - 1, MPI_INT, 0, MPI_COMM_WORLD);

		// Receive the calculated means and print them out
		float results[my_size];
		for(int i = 1; i < my_size; i++){
			results[i] = -1.0;
		}
 		
		float bullshit;
		MPI_Gather(&bullshit, 1, MPI_FLOAT, results, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
		for(int i = 1; i < my_size; i++){
			printf("The calculated mean number %d is %f\n", i, results[i]);
		}
 	
		/*
		float mean;
		MPI_Status status;
		for(int i = 1; i < my_size; i++){
			MPI_Recv(&mean, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
			printf("The calculated mean from process %d is %f\n", i, mean);
		} */		
				

	} else {
		// Receive the size of the data
		int my_data_len = - 1;
		MPI_Bcast(&my_data_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
		my_data_len--;

		// Receive data itself
		int my_data[my_data_len];
		MPI_Bcast(my_data, my_data_len, MPI_INT, 0, MPI_COMM_WORLD);

		// Calculate mean of data
		float my_mean = 0;
		for(int i = 0; i < my_data_len; i++){
			my_mean += (float) my_data[i];
		}
		my_mean /= (float) my_data_len;

		// MPI_Send(&my_mean, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		MPI_Gather(&my_mean, 1, MPI_FLOAT,NULL,	1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	
		
	}

	MPI_Finalize();
	return 0;
}
