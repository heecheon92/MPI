/*
Author: Heecheon Park
Description: Matrix multiplication in 1d array.
 */

import mpi.*;
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

class MPI_matmul {
    public static final int MASTER = 0;
    public static int = offset;
    public static int = local_row;

    public static double mat[] = new double[1000000];
    public static double mat2[] = new double[1000000];
    public static double gathered_output_mat = new double[1000000];
    public static double local_mat[];
    public static double local_output_mat[];

    static public void main(String[] args) throws MPIException {

        MPI.Init(args);

        int world_rank = MPI.COMM_WORLD.getRank();
        int world_size = MPI.COMM_WORLD.getSize() ;
        offset = 1000000 / world_size;
        local_row = 1000 / world_size;

        Scanner sc = new Scanner(new BufferedReader(new FileReader("1000x1000_matrix.txt")));
        while(sc.hasNextLine()) {
         for (int i=0; i<row; i++) {
            String[] line = sc.nextLine().trim().split(" ");
            for (int j=0; j<line.length; j++) {
               mat[i*1000+j] = Double.parseDouble(line[j]);
               mat2[i*1000+j] = Double.parseDouble(line[j]);
            }
         }
        }

        MPI.Finalize();
    }
}
