/*
Name: Heecheon Park
Simple MPI_Reduce program.
Execution method:

mpicc mpi_reduce.c -o mpi_reduce
mpirun -np number_of_processor ./mpi_reduce number_of_random_numbers
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

#define MASTER 0

int main(int argc, char* argv[])
{
	int world_rank, world_size;
	size_t arr_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	assert(argv[1] != NULL);
	int indexSize = atoi(argv[1]);
	if (indexSize == 2)
	{
		fprintf(stderr, "Please make sure that the program argument is larger than 2!\n");
		assert(atoi(argv[1]) != 2);
		exit(1);
	}
	float *arr = (float*) malloc(sizeof(float) * indexSize);
		
	arr_size = sizeof(arr) / sizeof(arr[0]);

	srand(time(NULL)*world_rank);
	for (int i = 0; i < indexSize; i++)
	{
		arr[i] = 1 + rand() % 100;
		// Just in case.
		assert(arr[i] != 0);
	}

	float local_sum = 0;
	for (int i = 0; i < indexSize; i++)
		local_sum += arr[i];

	// Print the random number on each process
	printf("Local sum for process %d - %.2f, avg = %.2f\n",
		world_rank, local_sum, local_sum / indexSize);

	float global_sum;
	// Reduce all of the local sums into the global sum.
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (world_rank == 0)	
		printf("Total sum %.2f, avg = %.2f\n", global_sum, global_sum / (world_size * indexSize));
	
	free(arr);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}

