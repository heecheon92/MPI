/*
Name: Heecheon Park
MPI Matrix multiplication in C.
Execution method:

mpicc mpi_matmul.c -o mpi_matmul
mpirun -np "number of processor" ./mpi_matmul

Warning: Use the number of processors so that it can be divisible to row.

If the row is 1000, then you should not use 3, 6, 7, or so forth as a number of processors.

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

#define MASTER 0
#define ROW 1000
#define COL 1000

void printMat ( double mat[1000][1000], int row, int col );
void print_1d_mat(double* mat, int row, int col);
void mat_mult_double_1d(double* mat1, int mat1_row, int mat1_col,
                        double mat2[], int mat2_row, int mat2_col,
                        double* output_mat, int output_mat_row, int output_mat_col);

int main(int argc, char* argv[])
{
	int world_rank, world_size;
	size_t arr_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
    // col_split is a measurement of how many row elements to be sent to each process.
    int col_split = 1000 / (world_size - 1);
    int chunkSize = col_split * 1000;

    static double mat[1000000];
    static double mat2[1000000];

    //static double local_mat[row_split][1000];
    static double *local_mat;
    static double *local_mat2;
    static double *local_output_mat;
    static double gathered_output_mat[1000000];

    int offset = 1000000 / world_size;
    int local_row = 1000 / world_size;
    double startTime, endTime;

    FILE *fp, *fp2;
    if (world_rank == MASTER)
    {
            fp = fopen("1000x1000_matrix.txt", "r");
            fp2 = fopen("1000x1000_matrix.txt", "r");

            if ((fp == NULL) || (fp2 == NULL))
            {
                fprintf(stderr, "File not found!\n");
                exit(-1);
            }

            for (int i = 0; i < 1000000; i++)
                fscanf(fp, "%lf", &mat[i]);

           
            for (int i = 0; i < 1000000; i++)
                fscanf(fp2, "%lf", &mat2[i]);

            fclose(fp);
            fclose(fp2);
    }
    printf("Everything has been read to the matrices.\n");

    //print_1d_mat(mat, 1000, 1000);
    local_mat = (double *) malloc(sizeof(double) * chunkSize);
    local_mat2 = (double *) malloc(sizeof(double) * 1000000);
    local_output_mat = (double *) malloc(sizeof(double) * 1000000);
    printf("Master has initiated sending.\n");

    startTime = MPI_Wtime();
    if (world_rank == MASTER)
    {
        printf("Master has initiated sending.\n");
        for (int process = 1; process < world_size; process++)
        {
            MPI_Send(&mat[(process-1) * chunkSize], chunkSize, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
            MPI_Send(&mat2, 1000000, MPI_DOUBLE, process, 2, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Send has been completed.\n");
    }
    
    if (world_rank != MASTER)
    {
        printf("Recv has been initiated.\n");
        for (int process = 1; process < world_size; process++)
        {
            MPI_Recv(local_mat, chunkSize, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(local_mat2, 1000000, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Recv has been completed.\n");
        //print_1d_mat(local_mat, 1000, col_split);
        mat_mult_double_1d(local_mat, col_split, 1000,
                           local_mat2, 1000, 1000,
                           local_output_mat, col_split, 1000);

        //print_1d_mat(local_output_mat, 1000, 1000);
        MPI_Send(local_output_mat, chunkSize, MPI_DOUBLE, MASTER, 3, MPI_COMM_WORLD);
        // Don't terminate the program until the subprocess's operation is completed.
        MPI_Barrier(MPI_COMM_WORLD);
    }
    endTime = MPI_Wtime();

    if (world_rank == MASTER)
    {
        for (int process = 1; process < world_size; process++)
        {
            //MPI_Recv(&gathered_output_mat, 1000000, MPI_DOUBLE, process, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&gathered_output_mat[(process-1) * chunkSize], chunkSize, MPI_DOUBLE, process, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        print_1d_mat(gathered_output_mat, 1000, 1000);
        //printf("Time measured: %3.9lf\n", endTime - startTime);
        fflush(stdout);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    free(local_mat);
    free(local_output_mat);

	MPI_Finalize();
    printf("Time measured: %3.9lf\n", endTime - startTime);
}


void printMat(double mat[1000][1000], int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < row; j++)
            printf("%.2f ", mat[i][j]);
        printf("\n");
    
    }
}

void print_1d_mat(double* mat, int row, int col)
{
    int i, k;
    k = 0;

    for ( i = 1; i <= row * col ; i++)
    {
        printf("%10.2lf ", mat[k]);
        if ((i % col) == 0 || i == ( row * col)) printf("\n");
        k++;
    }
    printf("Printing mat done....\n");
}

void mat_mult_double_1d(double* mat1, int mat1_row, int mat1_col,
                        double mat2[], int mat2_row, int mat2_col,
                        double* output_mat, int output_mat_row, int output_mat_col)
{
    for (int i = 0; i < mat1_row; i++)
        for (int j = 0; j < mat2_col; j++)
        {
            double sum = 0.0;
            for (int k = 0; k < mat2_row; k++)
                sum = sum + mat1[i * mat1_col + k] * mat2[k * mat2_col + j];
            output_mat[i * output_mat_col + j] = sum;
        }
}
