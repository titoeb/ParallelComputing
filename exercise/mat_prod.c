#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

struct matrix_t{
	int **values;
	int row_num;
	int col_num;
};


struct matrix_t *allocate_matrix_t(int *values /* row-major order */, int col_num , int row_num){
	struct matrix_t *this = malloc(sizeof(struct matrix_t));
	this->values = malloc(sizeof(int*) * col_num);
	for(int i = 0; i < col_num; i++){
		this->values[i] = malloc(sizeof(int) * row_num);
	}
	int count = 0;
	for(int col = 0; col < col_num; col++){
		for(int row = 0; row < row_num; row++){
			this->values[col][row] = values[count];
			count++;
		}
	}
	this->row_num = row_num;
	this->col_num = col_num;

	return this;
}

void free_matrix_t(struct matrix_t *this){
	for(int i = 0; i < this->col_num; i++){
		free(this->values[i]);
	}
	free(this->values);
	free(this);
}

void print_matrix_t(struct matrix_t *this){
	for(int col = 0; col < this->col_num; col++){
		for(int row = 0; row < this->row_num; row++){
			printf("%d ", this->values[col][row]);
		}
		printf("\n");
	}
}

int main(){

	MPI_Init(NULL, NULL);
	int my_rank, my_size;
	MPI_Comm_size(MPI_COMM_WORLD, &my_size);	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(my_rank == 0){
		// Main Process
		
		// Read in matrix A
		printf("Please enter dim of matrix A, B.\n");
		int dim;
		scanf("%d", &dim);
	
		printf("Entered dim was %d \n", dim);
		
		int values[dim * dim];
		
		// Create Matrix A
		for(int i = 0; i < dim * dim; i++){
			values[i] = rand() % 20;
		}

		struct matrix_t *A_main = allocate_matrix_t(values, dim, dim);
		printf("Matrix A is \n");
		print_matrix_t(A_main);
		printf("\n");
				
		// Create Matrix B
		for(int i = 0; i < dim * dim; i++){
			values[i] = rand() % 20;
		}

		struct matrix_t *B_main = allocate_matrix_t(values, dim, dim);
		printf("Matrix B is \n");
		print_matrix_t(B_main);
		printf("\n");
		

		// Communicate B to all workers
		// First communicate dimension
		MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

		// Send B
		MPI_Bcast(B_main->values, dim * dim, MPI_INT, 0, MPI_COMM_WORLD);

		// Send A
		MPI_Bcast(A_main->values, dim * dim, MPI_INT, 0, MPI_COMM_WORLD);

		// Receive Result
		
		
		// Free all matrices
		free_matrix_t(A_main);
		free_matrix_t(B_main);

	} else {
		// Worker Process
		int my_dim; // size of the quadratic matrix
		MPI_Bcast(&my_dim, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		int my_values_B[my_dim * my_dim];
		MPI_Bcast(my_values_B, my_dim * my_dim, MPI_INT, 0, MPI_COMM_WORLD);
		
		
	}
	

	
	MPI_Finalize();
	return 0;
}

