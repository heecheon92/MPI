#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>

#define MASTER 0


int main(int argc, char* argv[])
{
	int world_rank, world_size;
	int procs_iterator;
	size_t arr_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	double mat[16];
	double *firstHalf;
	double *secondHalf;
	
	arr_size = sizeof(mat) / sizeof(mat[0]);

	if ((arr_size % 2) != 0)
	{
		printf("Array size must be even number!\n");
		assert((arr_size % 2) == 0);
	}	
	for (int i = 0; i < 16; i++)
	{
		mat[i] = i+1;
	}

	firstHalf = malloc((arr_size / 2) * sizeof(mat[0]));
	secondHalf = malloc((arr_size / 2) * sizeof(mat[0]));

	memcpy(firstHalf, mat, (arr_size / 2) * sizeof(mat[0]));
	memcpy(secondHalf, mat + (arr_size / 2), (arr_size / 2) * sizeof(mat[0]));

	// Error Checking
	//for (int i = 0; i < 8; i++)
	//	printf("%.2f ", secondHalf[i]);
	//printf("\n");

	if (world_rank == MASTER)
	{
		printf("Master processor here!\n");
		printf("An array of 16 elements has been initailized.\n");
		for (int i = 0; i < 16; i++)
			printf("%.2f ", mat[i]);
		printf("\n");
		printf("Preparing to split the array and to send them to slave processors.\n");
		
		//for (procs_iterator = 1; procs_iterator < world_size; procs_iterator++)
		//{
		//	MPI_Send(
		//}	
		
		MPI_Send(firstHalf, (arr_size / 2), MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
		MPI_Send(secondHalf, (arr_size / 2), MPI_DOUBLE, 2, 2, MPI_COMM_WORLD);
		printf("Messages have been sent to processor 1 and 2 !!\n");
		printf("-----------------------------------------------------------------\n\n");
	}
	else if (world_rank == 1)
	{
		MPI_Recv(firstHalf, (arr_size / 2), MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received following data:\n", world_rank);
		for (int i = 0; i < (arr_size / 2); i++)
			printf("%.2f ", firstHalf[i]);
		printf("\n");
		printf("-----------------------------------------------------------------\n\n");
	}
	else if (world_rank == 2)
	{	
		MPI_Recv(secondHalf, (arr_size / 2), MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received following data:\n", world_rank);
		for (int i = 0; i < (arr_size / 2); i++)
			printf("%.2f ", secondHalf[i]);
		printf("\n");
		printf("-----------------------------------------------------------------\n\n");
	}

	
	MPI_Finalize();
}

