"""
Author: Heecheon Park

Note: Run python3 100x100_mat_generator.py first before running this program.

Description: Read numbers from 100x100_matrix.txt

Store each line into lists and numpy arrays.

Then, benchmarks matrix multiplication.

For example, list A and list B are 2-dimensional lists and

performs list A * list B.

Likewise, numpy A and numpy B are 2-dimensional numpy arrays and

performs numpy A * numpy B.

I created a matrix multiplication function that takes 2 array-like containers and output array.

The function calculate and append the result to output array.
"""
import numpy as np
import time
import sys

from array import array
#from timeit import timeit
from timeit import Timer

def main():
    
    list_mat = []
    list_mat2 = []
    np_mat = np.zeros((100,100), dtype=np.int64)
    np_mat2 = np.zeros((100,100), dtype=np.int64)
    
    # Initializing 100x100 list_matrix of zeros
    list_output = [[0 for col in range(100)] for rows in range(100)]
    np_output = np.zeros((100,100), dtype=np.int64)


    """
    Tried to implement Python's array module but I cannot find if multidimensional array is possible.
    I have only succeeded in making single dimension array with a specific datatype.
    """
    #array_mat = array('q', range(100))
    #array_mat2 = array('q', range(100))
    #array_output = array('q', range(100))
    #array_output = array_output.fromlist(array.fromlist(list_output))
    #for row in list_output: array_output.append(array.fromlist(row))
    #print("array output:", array_output)

    with open("100x100_matrix.txt", "r") as f:
        for line in f:
            # Split each line as a list of string
            int_string_list = line.split()

            # Convert the string element to int
            int_list = [int(i) for i in int_string_list]

            # Append the int_list to list_mat and list_mat2
            list_mat.append(int_list)
            list_mat2.append(int_list)

            #array_mat.append(array.fromlist(int_list))
            #array_mat2.append(array.fromlist(int_list))

        f.close()

    # New Discovery!!
    # dtype could be "dtype=int". but int is 32-bit based. So if an element gets too large,
    # it will become a negative integer.
    np_mat = np.loadtxt("100x100_matrix.txt", usecols=range(0, 100), dtype=np.int64)
    np_mat2 = np.loadtxt("100x100_matrix.txt", usecols=range(0, 100), dtype=np.int64)

    print("list_mat: ", list_mat)

    # Print large numpy arrays without truncation.
    np.set_printoptions(threshold=sys.maxsize)
    print("np_mat: \n", np_mat)

    """
        Following print statements display the results of matrix multiplication by list and numpy array.
        First and second statements display the results from my custom matrix multiplication function.
        Last statements display the result from built-in numpy matrix multiplication function.
        Results are quite large. So only use them when you want to compare results.
    """
    #print("Custom Mat_Multiplication (list):\n", mat_mult(list_mat, list_mat2, list_output))
    #print("Custom Mat_Multiplication (ndarray):\n", mat_mult(np_mat, np_mat2, np_output))
    #print("Numpy Built-in Mat_Multiplication (ndarray):\n", np.matmul(np_mat, np_mat2))
    
    """
        Benchmarking using timeit function but the function parameter cannot take arguments
    """
    #print("Custom Mat_Mult (list):", timeit("mat_mult(list_mat, list_mat2, list_ouput)", setup="from __main__ import mat_mult"))
    #print("Custom Mat_Mult (ndarray):", timeit("mat_mult(np_mat, np_mat2, np_ouput)", setup="from __main__ import mat_mult"))
    #print("Numpy Built-in Mat_Mult (ndarray):", timeit("np.matmul(np_mat, np_mat2)", setup="import numpy as np"))

    #%timeit mat_mult(list_mat, list_mat2, list_output)
    #%timeit mat_mult(np_mat, np_mat2, np_output)
    #%timeit np.matmul(np_mat, np_mat2)

    """
        Timer function cannot take function parameter with arguments which is uncallable.
        However, using lambda can make the function parameter with arguments callable.
    """
    list_timer = Timer(lambda: mat_mult(list_mat, list_mat2, list_output))
    ndarray_timer = Timer(lambda: mat_mult(np_mat, np_mat2, np_output))
    built_in_mult_timer = Timer(lambda: np.matmul(np_mat, np_mat2, np_output))

    print("*"*80)
    iteration_count = print("How many times would you like to perform the matrix multiplication?")
    iteration_count = int(input("I recommend a small number like less than 50: "))
    print("*"*80)
    print("Custom Mat_Multiplication {} times (list):".format(iteration_count), list_timer.timeit(number=iteration_count))
    print("Custom Mat_Multiplication {} times (ndarray):".format(iteration_count), ndarray_timer.timeit(number=iteration_count))
    print("Numpy Built-in Mat_Multiplication {} times (array):".format(iteration_count), built_in_mult_timer.timeit(number=iteration_count))

def mat_mult(mat1, mat2, output_mat):
    
    row = len(mat1)
    col = len(mat1[0])

    for r in range(0, row):
        for c in range(0, col):
            for r_iter in range(0, row):                
                output_mat[r][c] += mat1[r][r_iter] * mat2[r_iter][c] 

    return output_mat



if __name__ == "__main__":
    main()
