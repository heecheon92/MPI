/*
Author: Heecheon Park

This file is experimental to provide MPI functions
to be called from Python including a few utility functions like matrix multiplication.

Build:
    $gcc -shared -o c_pylib.so -fPIC c_pylib.c
    
    or if you need MPI functionalities, 
    
    $mpicc.mpich -shared -o c_pylib.so -fPIC c_pylib.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

void print_1d_mat(double* mat, int row, int col);
//int MPI_Init( NULL, NULL );
//int MPI_Finalize(void);
//int MPI_Comm_rank( MPI_Comm comm, int *rank );
//int MPI_Comm_size(MPI_Comm comm, int *size);

MPI_Comm getCommunicator()
{
    return MPI_COMM_WORLD;
}

MPI_Status* getAnyStatus()
{
    return MPI_STATUS_IGNORE;
}

void mpi_send(void* data, int count, int destination, int tag, MPI_Comm COMM)
{
    MPI_Send(data, count, MPI_DOUBLE, destination, tag, COMM);
}


void mpi_recv(void* data, int count, int source, int tag, MPI_Comm COMM)
{
    MPI_Recv(data, count, MPI_DOUBLE, source, tag, COMM, MPI_STATUS_IGNORE);
}

void matmul(double* matA, int rowA, int colA, 
            double* matB, int rowB, int colB, 
            double* matC, int rowC, int colC) 
{
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            double sum = 0.0;
            for (int k = 0; k < rowB; k++)
                sum = sum + matA[i * colA + k] * matB[k * colB + j];
            matC[i * colC + j] = sum;
        }
    }
}

void mpi_matmul(double* matA, int rowA, int colA, 
                   double* matB, int rowB, int colB, 
                   double* matC, int rowC, int colC) 
{
    int world_rank, 
        world_size,
        col_split,
        chunkSize;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    col_split = rowA / (world_size - 1);
    chunkSize = col_split * rowA;

    static double *local_mat;
    static double *local_mat2;
    static double *local_output_mat;

    //double gathered_output_mat[rowC * colC];

    local_mat = (double*) malloc(sizeof(double) * chunkSize);
    local_mat2 = (double*) malloc(sizeof(double) * (rowB * colB));
    local_output_mat = (double*) malloc(sizeof(double) * (rowC * colC));


    if (world_rank == MASTER)
    {
        //for (int i = 0; i < 5; i++)
        //    printf("Double checking my array: %lf\n", matA[i]);
        printf("Master has initiated sending.\n");
        for (int process = 1; process < world_size; process++)
        {
            MPI_Send(&(matA)[(process-1) * chunkSize], chunkSize, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
            MPI_Send(matB, rowB * colB, MPI_DOUBLE, process, 2, MPI_COMM_WORLD);
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
            MPI_Recv(local_mat2, rowB * colB, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Recv has been completed.\n");
        matmul(local_mat, col_split, colA,
               local_mat2, rowB, colB,
               local_output_mat, col_split, colB);

        MPI_Send(local_output_mat, chunkSize, MPI_DOUBLE, MASTER, 3, MPI_COMM_WORLD);
        // Don't terminate the program until the subprocess's operation is completed.
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
        //MPI_Abort(MPI_COMM_WORLD, -1);
    }

    if (world_rank == MASTER)
    {
        printf("Collecting all data at MASTER.\n");
        for (int process = 1; process < world_size; process++)
        {
            MPI_Recv(&matC[(process-1) * chunkSize], chunkSize, MPI_DOUBLE, process, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("Collected all data at MASTER.\n");
        MPI_Barrier(MPI_COMM_WORLD);
        print_1d_mat(matC, 512, 512);
        fflush(stdout);
    }

    free(local_mat);
    free(local_output_mat);

    MPI_Finalize();
    //for (int i = 0; i < 5; i++)
    //    printf("Double checking my array: %lf\n", matC[i]);
    //if (world_rank == MASTER)
    //    return matC;
}

int hello_mpi() {
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    MPI_Finalize();
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
