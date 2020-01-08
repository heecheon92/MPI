/*
Author: Heecheon Park
Description: Matrix multiplication in 1d array.
 */

import mpi.*;
import java.util.*;
import java.io.*;
import static mpi.MPI.slice;


class MPI_matmul_2d {

    /*
    * Declare static variables / constants in class.
    * not in methods.
    */
    public static final int MASTER = 0;
    public static final int NRA = 1000;
    public static final int NCA = 1000;
    public static final int NCB = 1000;
    public static double[][] mat = new double[NRA][NCA];
    public static double[][] mat2 = new double[NRA][NCB];
    public static double[][] output_mat = new double[NRA][NCB];
    public static int rank,
                      size,
                      numworkers,
                      rows,
                      source,
                      dest,
                      averow,
                      extra,
                      offset,
                      i, j, k, rc;

    static public void main(String[] args) throws MPIException {

        MPI.Init(args);

        rank = MPI.COMM_WORLD.getRank();
        size = MPI.COMM_WORLD.getSize() ;
        Status status;

        numworkers = size - 1;

        if (rank == MASTER)
        {
            try
            {
                Scanner sc = new Scanner(
                             new BufferedReader(
                             new FileReader("1000x1000_matrix.txt")));
                while (sc.hasNextLine())
                {
                    for (i = 0; i < 1000; i++)
                    {
                        String[] line = sc.nextLine().trim().split(" ");
                        for (j = 0; j < line.length; j++)
                        {
                            mat[i][j] = Double.parseDouble(line[j]);
                            mat2[i][j] = Double.parseDouble(line[j]);
                            output_mat[i][j] = 0;
                        }
                    }
                }
            }
            catch(FileNotFoundException fnfe)
            {
                fnfe.printStackTrace();
            }

            averow = NRA / numworkers;
            extra = NRA % numworkers;
            offset = 0;

            for (dest = 1; dest <= numworkers; dest++)
            {
                rows = (dest <= extra) ? averow + 1 : averow;

                System.out.println("Sending " + rows + " rows to task " + dest + " offset = " + offset);
                
                // Do not send offset if the offset is 0.
                // https://salsa.debian.org/jrtc27/openmpi/blob/upstream/README.JAVA.txt

                //if (offset != 0)
                //    MPI.COMM_WORLD.send(offset, 1, MPI.INT, dest, 1);

                MPI.COMM_WORLD.send(rows, 1, MPI.INT, dest, 1);
                MPI.COMM_WORLD.send(slice(mat, offset)[0], rows * NCA, MPI.DOUBLE, dest, 1);
                //MPI.COMM_WORLD.send(mat[offset][0], rows * NCA, MPI.DOUBLE, dest, 1);
                MPI.COMM_WORLD.send(mat2, rows * NCA, MPI.DOUBLE, dest, 1);
                offset = offset + rows;
            }

            for (i = 1; i <= numworkers; i++)
            {
                source = 1;
                MPI.COMM_WORLD.recv(offset, 1, MPI.INT, source, 2);
                MPI.COMM_WORLD.recv(rows, 1, MPI.INT, source, 2);
                //MPI.COMM_WORLD.recv(output_mat[offset][0], rows * NCB, MPI.INT, source, 2);
                MPI.COMM_WORLD.recv(slice(output_mat, offset)[0], rows * NCB, MPI.INT, source, 2);
            }

            for (i = 0; i < NRA; i++)
            {
                System.out.println();
                for (j = 0; j < NCB; j++)
                    System.out.println(" " + output_mat[i][j]);
                System.out.println();
            }

        }

        if (rank > MASTER)
        {
            if (offset != 0 && rows != 0)
            {
                MPI.COMM_WORLD.send(offset, 1, MPI.INT, MASTER, 1);
                MPI.COMM_WORLD.send(rows, 1, MPI.INT, MASTER, 1);
            }
            MPI.COMM_WORLD.send(mat, rows * NCA, MPI.DOUBLE, MASTER, 1);
            MPI.COMM_WORLD.send(mat2, NCA * NCB, MPI.DOUBLE, MASTER, 1);

            for (k = 0; k < NCB; k++)
                for (i = 0; i < rows; i++)
                {
                    output_mat[i][j] = 0.0;
                    for (j = 0; j < NCA; j++)
                        output_mat[i][k] = output_mat[i][k] + mat[i][j] * mat2[j][k];
                }
            MPI.COMM_WORLD.send(offset, 1, MPI.INT, MASTER, 2);
            MPI.COMM_WORLD.send(rows, 1, MPI.INT, MASTER, 2);
            MPI.COMM_WORLD.send(output_mat, 1, MPI.INT, MASTER, 2);
        }

        if (rank == MASTER)
            printMat(output_mat, 1000, 1000);

        MPI.Finalize();
    }

    public static void printMat(double[][] mat, int row, int col)
    {
        for (int i= 0; i < row; i++)
        {
            for (int j = 0; j < row; j++)
                {
                    System.out.print(" " + mat[i][j]);
                }
            System.out.println();
        }
    }
}
