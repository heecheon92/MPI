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
#define NRA 1000
#define NCA 1000
#define NCB 1000

void printMat ( double mat[1000][1000], int row, int col );
void print_1d_mat(double* mat, int row, int col);
void mat_mult_double_1d(double* mat1, int mat1_row, int mat1_col,
                        double mat2[], int mat2_row, int mat2_col,
                        double* output_mat, int output_mat_row, int output_mat_col);

int main(int argc, char* argv[])
{
	int world_rank, 
        world_size,
        numworkers,
        rows,
        source,
        dest,
        averow, extra, offset,
        i, j, k, rc;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Status status;
	
    int row_split = 1000 / world_size;
    static double mat[NRA][NCA];
    static double mat2[NRA][NCB];
    static double output_mat[NRA][NCB];

    numworkers = world_size - 1;
    //static double local_mat[row_split][1000];
    //static double *local_mat;
    //static double *local_output_mat;
    //static double gathered_output_mat[1000000];

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

            for (i = 0; i < 1000; i++)
                for (j = 0; j < 1000; j++)
                    fscanf(fp, "%lf", &mat[i][j]);

            for (i = 0; i < 1000; i++)
                for (j = 0; j < 1000; j++)
                    fscanf(fp2, "%lf", &mat2[i][j]);

            fclose(fp);
            fclose(fp2);

            averow = NRA / numworkers;
            extra = NRA % numworkers;
            offset = 0;

            for (dest=1; dest<=numworkers; dest++)
            {
               rows = (dest <= extra) ? averow+1 : averow;   	
               printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
               MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
               MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
               MPI_Send(&mat[offset][0], rows*NCA, MPI_DOUBLE, dest, 1,
                         MPI_COMM_WORLD);
               MPI_Send(&mat2, NCA*NCB, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
               offset = offset + rows;
            }

            /* Receive results from worker tasks */
            for (i=1; i<=numworkers; i++)
            {
               source = i;
               MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
               MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
               MPI_Recv(&output_mat[offset][0], rows*NCB, MPI_DOUBLE, source, 2, 
                        MPI_COMM_WORLD, &status);
               printf("Received results from task %d\n",source);
            }

            /* Print results */
            printf("******************************************************\n");
            printf("Result Matrix:\n");
            for (i=0; i<NRA; i++)
            {
               printf("\n"); 
               for (j=0; j<NCB; j++) 
                  printf("%6.2f   ", output_mat[i][j]);
            }
            printf("\n******************************************************\n");
            printf ("Done.\n");
    }

   if (world_rank > MASTER)
   {
      MPI_Recv(&offset, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&mat, rows*NCA, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&mat2, NCA*NCB, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, &status);

      for (k=0; k<NCB; k++)
         for (i=0; i<rows; i++)
         {
            output_mat[i][k] = 0.0;
            for (j=0; j<NCA; j++)
               output_mat[i][k] = output_mat[i][k] + mat[i][j] * mat2[j][k];
         }
      MPI_Send(&offset, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
      MPI_Send(&output_mat, rows*NCB, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD);
   }

    if (world_rank == MASTER)
    {
        printMat(output_mat, 1000, 1000);
    }
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
