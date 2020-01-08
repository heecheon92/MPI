/*
Author: Heecheon Park
Description: Matrix multiplication in 1d array.
 */

import mpi.*;
import static mpi.MPI.slice;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.File;
import java.io.FileReader;
import java.util.Scanner;
import java.lang.System;
import java.util.Date;
import java.text.DecimalFormat;

class MPI_matmul_sr_512 {
    public static final int MASTER = 0;
    public static int offset;
    public static int local_row;
    public static int col_split;
    public static int chunkSize;

    public static double mat[] = new double[262144];
    public static double mat2[] = new double[262144];
    public static double gathered_output_mat[] = new double[262144];
    public static double local_mat[];
    public static double local_mat2[];
    public static double local_output_mat[];

    static public void main(String[] args) throws MPIException {

        MPI.Init(args);

        int world_rank = MPI.COMM_WORLD.getRank();
        int world_size = MPI.COMM_WORLD.getSize() ;

        double startTime = 0.0;
        double endTime = 0.0;

        offset = 262144 / world_size;
        local_row = 1000 / world_size;

        col_split = 512 / (world_size - 1);
        chunkSize = col_split * 512;

        if (world_rank == MASTER)
        {
            try
            {
                Scanner sc = new Scanner(new BufferedReader(new FileReader("512x512_matrix.txt")));
                while(sc.hasNextLine()) {
                    for (int i=0; i<512; i++) {
                        String[] line = sc.nextLine().trim().split(" ");
                        for (int j=0; j<line.length; j++) {
                            mat[i*512+j] = Double.parseDouble(line[j]);
                            mat2[i*512+j] = Double.parseDouble(line[j]);
                        }
                    }
                }
            }
            catch(FileNotFoundException fnfe){
                fnfe.printStackTrace();
            }
        }

        local_mat = new double[chunkSize];
        local_mat2 = new double[262144];
        local_output_mat = new double[262144];

        //MPI.COMM_WORLD.barrier();
        //startTime = MPI.wtime();
        if (world_rank == MASTER)
        {
            startTime = MPI.wtime();
            //System.out.println("Master has initiated sending.");
            for (int process = 1; process < world_size; process++)
            {
                MPI.COMM_WORLD.send(slice(mat, (process - 1) * chunkSize), chunkSize, MPI.DOUBLE, process, 1);
                MPI.COMM_WORLD.send(mat2, 262144, MPI.DOUBLE, process, 2);
            }
            //MPI.COMM_WORLD.barrier();
            //System.out.println("Send has been completed.");
        }

        if (world_rank != MASTER)
        {
            //System.out.println("Recv has been initiated.");
            //for (int process = 1; process < world_size; process++)
            //{
                MPI.COMM_WORLD.recv(local_mat, chunkSize, MPI.DOUBLE, MASTER, 1);
                MPI.COMM_WORLD.recv(local_mat2, 262144, MPI.DOUBLE, MASTER, 2);
            //}
            //MPI.COMM_WORLD.barrier();
            //System.out.println("Recv has been completed.");
            mat_mult_double_1d(local_mat, col_split, 512,
                               local_mat2, 512, 512,
                               local_output_mat, col_split, 512);

            MPI.COMM_WORLD.send(local_output_mat, chunkSize, MPI.DOUBLE, MASTER, 3);
            //print_1d_mat(local_output_mat, col_split, 512);
        }

        if (world_rank == MASTER)
        {
            for (int process = 1; process < world_size; process++)
            {
                MPI.COMM_WORLD.recv(slice(gathered_output_mat, (process - 1) * chunkSize), chunkSize, MPI.DOUBLE, process, 3);
            }
            //print_1d_mat(gathered_output_mat, 512, 512);
            endTime = MPI.wtime();
        }
        //MPI.COMM_WORLD.barrier();
        //endTime = MPI.wtime();
        MPI.Finalize();
        if (world_rank == MASTER)
            System.out.println(endTime - startTime);
    }

    public static void print_1d_mat(double[] mat, int row, int col)
    {
        int i, k;
        k = 0;

        for (i = 1; i <= row * col; i++)
        {
            System.out.printf("%.0f ", mat[k]);
            if ((i % col) == 0 || i == ( row * col))
                System.out.println();
            k++;
        }
        //System.out.println("Printing mat done...");
    }

    public static void mat_mult_double_1d(double mat1[], int mat1_row, int mat1_col,
                                          double mat2[], int mat2_row, int mat2_col,
                                          double output_mat[], int output_mat_row, int output_mat_col)
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

}
