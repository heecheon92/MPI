"""
Author: Heecheon Park

Note: Run python3 1000x1000_mat_generator.py first before running this program.

Description: Read numbers from 1000x1000_matrix.txt

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
    np_mat = np.zeros((1000,1000), dtype=np.int64)
    np_mat2 = np.zeros((1000,1000), dtype=np.int64)
    np_mat_float = np.zeros((1000,1000), dtype=np.float32)
    np_mat_float2 = np.zeros((1000, 1000), dtype=np.float32)
    np_mat_double = np.zeros((1000,1000), dtype=np.float64)
    np_mat_double2 = np.zeros((1000, 1000), dtype=np.float64)
    np_mat_longdouble = np.zeros((1000,1000), dtype=np.longdouble)
    np_mat_longdouble2 = np.zeros((1000, 1000), dtype=np.longdouble)

    """
        Numpy does not support float 96 and 128 but numpy.longdouble which is float 96 and float 128
        np.finfo(np.longdouble) provides which precision my system uses. (Either 96 or 128)
    """
    #np_mat_float96 = np.zeros((1000,1000), dtype=np.float96)
    #np_mat_float96_2 = np.zeros((1000, 1000), dtype=np.float96)
    #np_mat_float128 = np.zeros((1000,1000), dtype=np.float128)
    #np_mat_float128_2 = np.zeros((1000, 1000), dtype=np.float128)


    # Initializing 1000x1000 list_matrix of zeros
    list_output = [[int(0) for col in range(1000)] for rows in range(1000)]
    np_output = np.zeros((1000,1000), dtype=np.int64)
    np_output_float = np.zeros((1000,1000), dtype=np.float32)
    np_output_double = np.zeros((1000,1000), dtype=np.float64)
    np_output_longdouble = np.zeros((1000,1000), dtype=np.longdouble)
    #np_output_float96 = np.zeros((1000,1000), dtype=np.float96)
    #np_output_float128 = np.zeros((1000,1000), dtype=np.float128)


    """
    Tried to implement Python's array module but I cannot find if multidimensional array is possible.
    I have only succeeded in making single dimension array with a specific datatype.
    """
    #array_mat = array('q', range(1000))
    #array_mat2 = array('q', range(1000))
    #array_output = array('q', range(1000))
    #array_output = array_output.fromlist(array.fromlist(list_output))
    #for row in list_output: array_output.append(array.fromlist(row))
    #print("array output:", array_output)

    with open("1000x1000_matrix.txt", "r") as f:
        for line in f:
            # Split each line as a list of string
            int_string_list = line.split()

            # Convert the string element to int or float
            int_list = [int(i) for i in int_string_list]
            #float_list = [float(i) for i in int_string_list]

            # Append the int_list to list_mat and list_mat2
            list_mat.append(int_list)
            list_mat2.append(int_list)
            #list_mat.append(float_list)
            #list_mat2.append(float_list)

    #        #array_mat.append(array.fromlist(int_list))
    #        #array_mat2.append(array.fromlist(int_list))

        f.close()

    # New Discovery!!
    # dtype could be "dtype=int". but int is 32-bit based. So if an element gets too large,
    # it will become a negative integer.
    np_mat = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.int64)
    np_mat2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.int64)
    np_mat_float = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float32)
    np_mat_float2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float32)
    np_mat_double = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float64)
    np_mat_double2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float64)
    np_mat_longdouble = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.longdouble)
    np_mat_longdouble2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.longdouble)
    #np_mat_float96 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float96)
    #np_mat_float96_2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float96)
    #np_mat_float128 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float128)
    #np_mat_float128_2 = np.loadtxt("1000x1000_matrix.txt", usecols=range(0, 1000), dtype=np.float128)


    #print("list_mat: ", list_mat)

    # Print large numpy arrays without truncation.
    #np.set_printoptions(threshold=sys.maxsize)
    #print("np_mat: \n", np_mat)

    """
        Accessing numpy array with Python defined function is extremely inefficient.
        Do not uncomment mat_mult function more than 1 for 1000x1000 matrix multiplication.
        If you are going to uncomment one mat_mult function, you may want to go grab a
        cup of coffee while this portion is being done :).
    """
    print("Calculation begins here.....")
    mat_mult(list_mat, list_mat2, list_output)
    #mat_mult(np_mat, np_mat2, np_output)
    #mat_mult(np_mat_float, np_mat_float2, np_output_float)
    #mat_mult(np_mat_double, np_mat_double2, np_output_double)
    #mat_mult(np_mat_longdouble, np_mat_longdouble2, np_output_longdouble)
    #mat_mult(np_mat_float96, np_mat_float96_2, np_output_float96)
    #mat_mult(np_mat_float128, np_mat_float128_2, np_output_float128)

    start_time = time.time()
    np.matmul(np_mat, np_mat2, np_output)
    end_time = time.time()
    print("np.matmul (int64) took {} seconds.".format(end_time - start_time))

    start_time = time.time()
    np.matmul(np_mat_float, np_mat_float2, np_output_float)
    end_time = time.time()
    print("np.matmul (float) took {} seconds.".format(end_time - start_time))

    start_time = time.time()
    np.matmul(np_mat_double, np_mat_double2, np_output_double)
    end_time = time.time()
    print("np.matmul (double) took {} seconds.".format(end_time - start_time))

    start_time = time.time()
    np.matmul(np_mat_longdouble, np_mat_longdouble2, np_output_longdouble)
    end_time = time.time()
    print("np.matmul (longdouble) took {} seconds.".format(end_time - start_time))


def mat_mult(mat1, mat2, output_mat):
    
    row = len(mat1)
    col = len(mat1[0])
    start_time = time.time()
    for r in range(0, row):
        for c in range(0, col):
            for r_iter in range(0, row):                
                output_mat[r][c] += mat1[r][r_iter] * mat2[r_iter][c] 
    
    end_time = time.time()
    print(type(output_mat[0][0]), "took","--- %s seconds ---" % (end_time - start_time))

    return output_mat



if __name__ == "__main__":
    main()
