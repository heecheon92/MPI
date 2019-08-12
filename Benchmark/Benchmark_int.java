/*
Name: Heecheon Park

This standalone java program read matrix from 1000x1000_matrix.txt

and performs a matrix multiplication.

Then, it measures how fast it calculates to compare with C++ and Python implementation on a single processor.

*/
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
//import java.io.FileInputStream;
//import java.io.BufferedInputStream;
//import java.util.Arrays;
//import java.util.concurrent.TimeUnit;

public class Benchmark_int {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        
        int row = 1000;
        int col = 1000;
        int[][] mat_int = new int[row][col];
        int[][] mat_int2 = new int[row][col];
        int[][] mat_int_output = new int[row][col];

        //FileInputStream fis = new FileInputStream("100x100_matrix.txt");
        //BufferedInputStream bis = new BufferedInputStream(fis);

        //int buf[];
        //int i = 0;
        //int content;
        //int row_iter = 0;
        //int col_iter = 0;
        //while ((content = bis.read()) != -1)
        //{
            //System.out.print((char) content);
            //mat_int[row_iter][col_iter] = (int) content;
            //if (col_iter % 100 == 0)
            //{
            //    row_iter++;
            //    col_iter = 0;
            //}
            //col_iter++;
        //}
        //System.out.println(Arrays.deepToString(mat_int));

        //for (int r = 0; r < row; r++)
        //    
        //    for (int c = 0; c < col; c++)
        //    {
        //        mat_int[r][c] = buf[iterator];
        //        iterator++;
        //    }       

        //fis.close();

        Scanner sc = new Scanner(new BufferedReader(new FileReader("1000x1000_matrix.txt")));
        while(sc.hasNextLine()) {
         for (int i=0; i<mat_int.length; i++) {
            String[] line = sc.nextLine().trim().split(" ");
            for (int j=0; j<line.length; j++) {
               mat_int[i][j] = Integer.parseInt(line[j]);
               mat_int2[i][j] = Integer.parseInt(line[j]);
               mat_int_output[i][j] = 0;
            }
         }
        }

        mat_mult_int(mat_int, mat_int2, mat_int_output);

        //for (int i = 0; i < 100; i++)
        //{
        //    mat_mult_int(mat_int, mat_int2, mat_int_output);
        //}
    }

    public static void mat_mult_int(int[][] mat1, int[][] mat2, int[][] output_mat_int)
    {
        //row = mat1.length;
        //col = mat1[0].length;
        //DecimalFormat df = new DecimalFormat("#0.00000000");
        int row = 1000;
        int col = 1000;
        long startTime = System.nanoTime();
        for (int r = 0; r < row; r++)
            for (int c = 0; c < col; c++)
                for (int r_iter = 0; r_iter < row; r_iter++)
                    output_mat_int[r][c] += mat1[r][r_iter] * mat2[r_iter][c];
        long endTime = System.nanoTime();
        System.out.println("Mat[1000][1000] * Mat2[1000][1000] took:");
        System.out.println((endTime - startTime)/1000000000.0 + " seconds");
       
    }

    public static void print_mat (int[][] mat, int row, int col ) 
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                System.out.println(mat[i][j]);
            System.out.println();
        }    
                
    }

 }
