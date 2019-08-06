/*
Name: Heecheon Park
Simple MPI_Scatter and MPI_Gather
Execution method:

mpicc mpi_scatter_and_gather.c -o mpi_scatter_and_receive
mpirun -np 2 ./mpi_scatter_and_gather 16

This program is not dynamic so the argument must be 16.

The master processor will generate an array of 16 random float.
Then, the master will scatter 8 float elements to each subprocessor.
Each subprocessor will print the received array.
Then, the master will gather all subarrays from subprocessors.
Finally, the master will print the end-output array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

#define MASTER 0

void print_mat ( float *mat, int row, int col );

int main(int argc, char* argv[])
{
	int world_rank, world_size;
	size_t arr_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	assert(argv[1] != NULL);
	int indexSize = atoi(argv[1]);
	if (indexSize != 16)
	{
		fprintf(stderr, "Please make sure that the program argument must be 16!\n");
		assert(atoi(argv[1]) == 16);
		exit(1);
	}
	float *arr = (float*) malloc(sizeof(float) * indexSize);
	float *local_arr = (float *) malloc(sizeof(float) * indexSize / 2);
	float *arr_gathered = (float*) malloc(sizeof(float) * indexSize);
		
	arr_size = sizeof(arr) / sizeof(arr[0]);

	for (int i = 0; i < indexSize; i++)
	{
		arr[i] = 1 + rand() % 100;
		// Just in case.
		assert(arr[i] != 0);
	}

	float local_sum = 0;
	for (int i = 0; i < indexSize; i++)
		local_sum += arr[i];

	if (world_rank == MASTER)
	{
		printf("Array at master processor:\n");
		print_mat(arr, 4, 4);
	}

	// Reduce all of the local sums into the global sum.
	MPI_Scatter(arr, 8, MPI_FLOAT, local_arr, 8, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
	printf("Array from the master processor has been scattered!\n");
	printf("*******************************************\n");
	printf("local array at processor %d:\n", world_rank);
	print_mat(local_arr, 4, 2);

	printf("*******************************************\n");
	printf("All the local arrays wil be gathered.\n");
	float * sub_arr = NULL;
	MPI_Gather(local_arr, 8, MPI_FLOAT, arr_gathered, 8, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
	if (world_rank == MASTER)
	{
		printf("Gathering has been completed!\n");
		printf("*******************************************\n");
		printf("Gathered array: \n");
		print_mat(arr_gathered, 4, 4);
	}
	if (world_rank == MASTER)
	{
		free(arr);
		free(arr_gathered);
		free(local_arr);
	}

	MPI_Finalize();
}

void print_mat ( float *mat, int row, int col ) {

  int i, k ;

  k = 0 ;
  for ( i = 1 ; i <= row*col ; i++ ) {

    printf( "%5.1f", mat[k] ) ;
    if ( ( i % row ) == 0 || i == ( row * col ) ) printf( "\n" ) ;
    k++ ;
  }
}

