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
	
    int row_split = 1000 / world_size;
    static double mat[1000000];
    static double mat2[1000000];

    //static double local_mat[row_split][1000];
    static double *local_mat;
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

    //print_1d_mat(mat, 1000, 1000);
    local_mat = (double *) malloc(sizeof(double) * offset);
    local_output_mat = (double *) malloc(sizeof(double) * offset);
    
    
    startTime = MPI_Wtime();
	MPI_Scatter(mat, offset, MPI_DOUBLE, local_mat, offset, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(mat2, 1000000, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    //print_1d_mat(local_mat, (1000 / world_size), 1000);
    printf("Before the multiplication.\n");
    mat_mult_double_1d(local_mat, local_row, 1000,
                       mat2, 1000, 1000,
                       local_output_mat, local_row, 1000);
    printf("After the multiplication.\n");
    //print_1d_mat(local_output_mat, (1000 / world_size), 1000);
    MPI_Gather(local_output_mat, local_row * 1000, MPI_DOUBLE, 
               gathered_output_mat, local_row * 1000, MPI_DOUBLE, 
               MASTER, MPI_COMM_WORLD);
    endTime = MPI_Wtime();

    if (world_rank == MASTER)
    {
        print_1d_mat(gathered_output_mat, 1000, 1000);
        printf("Time measured: %1.2lf\n", endTime - startTime);
        fflush(stdout);
    }
    free(local_mat);
    free(local_output_mat);

	MPI_Finalize();
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
