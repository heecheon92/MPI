// Author:Joshua DeNio
// Date: 8/8/19

/*
Modifier: Heecheon Park
Description: The original program performs 100x100 matrix multiplication 
with integer (32 bit) based on statically allocated 2d array. 
It works perfectly if the array size is not too large.
However, if the array size is too large, say 1000x1000, the stack overflow occurs and
the compiler complains with seg fault.
I have modified the program to allow integer value larger than 2,147,483,647 (default max int).
This program can hold integer up to ((2^63)-1).

This way, I should be able to benchmark 1000x1000 matrix multiplication on Java, Python, and C++
*/

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

void mat_mult(long long int ** mat1, long long int ** mat2, long long int ** output_mat);

int main(){
    
    long long int ** list_mat;
    list_mat = new long long int*[1000];

    long long int ** list_mat2;
    list_mat2 = new long long int*[1000];

    long long int ** output_mat;
    output_mat = new long long int*[1000];

    for (int i = 0; i < 1000; i++)
    {
        list_mat[i] = new long long int[1000];
        list_mat2[i] = new long long int[1000];
        output_mat[i] = new long long int[1000];
    }

    int currentTime;
    
    int rowCount = 0;
    int colCount = 0;
    
    ifstream inFile("1000x1000_matrix.txt");
    if (inFile.is_open()){
        
        for (int i = 0; i < 1000; i++){
            //
            for (int j =0; j < 1000; j++){
                inFile >> list_mat[i][j];
            }
        }
    }
    inFile.close();
    
    ifstream inFile2("1000x1000_matrix.txt");
    if (inFile2.is_open()){
        
        for (int i = 0; i < 1000; i++){
            //
            for (int j =0; j < 1000; j++){
                inFile2 >> list_mat2[i][j];
            }
        }
    }
    inFile2.close();
    
    mat_mult(list_mat, list_mat2, output_mat);

    for (int i = 0; i < 1000; i++)
    {
        delete [] list_mat[i];
        delete [] list_mat2[i];
        delete [] output_mat[i];
    }

    delete [] list_mat;
    delete [] list_mat2;
    delete [] output_mat;
    
    return 0;
}

void mat_mult(long long int **mat1, long long int **mat2, long long int **output_mat){
    
    int row = 1000;
    int col = 1000;
    
    chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
    
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            for (int r_iter = 0; r_iter < row; r_iter++){                
                output_mat[r][c] += mat1[r][r_iter] * mat2[r_iter][c];
            }
        }
    }
    
    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> timeElapsed = endTime - startTime;
    
    cout << "Matrix multiplication of two" << endl;
    cout << "Integer matrix of type matrix[1000][1000]" << endl;
    cout << "\n";
    
    cout << "The processing time is: ";
    cout << timeElapsed.count() << " in seconds" << endl;
}
