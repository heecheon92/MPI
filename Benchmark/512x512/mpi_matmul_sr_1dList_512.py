"""
Name: Heecheon Park
Date: September 6th 2019
Minnesota State University Moorhead

Running 1000x1000 matrix multiplication with list and mpi
with 5 processors.


Execution Method:

mpiexec -np 5 python3 mpi_list_matmult.py
"""
from mpi4py import MPI
from numba import njit, jit
import numpy as np
import sys
import time

MASTER = 0

def main():
        
    COMM = MPI.COMM_WORLD
    RANK = COMM.Get_rank()
    SIZE = COMM.Get_size()

    #print(SIZE)

    col_split = 512 // (SIZE - 1)
    chunkSize = int(col_split) * 512
    offset = 252144 / (SIZE - 1)
    local_row = 1000 / (SIZE - 1)

    list_mat = []
    list_mat2 = []
    local_mat = []
    local_mat2 = []
    local_output_mat = []
    gathered_output_mat = []


    startTime = 0.0
    endTime = 0.0
    
    #print("MPI Initiated")
    with open("512x512_matrix.txt",'r') as f:
        for line in f:
            int_string_list = line.split()
            float_list = [float(i) for i in int_string_list]
            for i in float_list:
                list_mat.append(i)
                list_mat2.append(i)

        #print("Read from file done.")
        f.close()

    if RANK == MASTER:
        startTime = MPI.Wtime()
        for process in range(1, SIZE):
            COMM.send(list_mat[(process-1) * chunkSize:], process, 1)
            COMM.send(list_mat2, process, 2)
        #print("Send from Master done.")

    if RANK != MASTER:
        local_mat = COMM.recv(source=MASTER, tag=1)
        local_mat2 = COMM.recv(source=MASTER, tag=2)
        #print("Recv from client done.")
        local_output_mat = [0 for zero in range(col_split * 512)]
        mat_mult_1d(local_mat, col_split, 512, local_mat2, 512, 512, local_output_mat, col_split, 512)
        #print(local_output_mat[(col_split * 512) - 1])
        COMM.send(local_output_mat, MASTER, 3)
        #print("Send from client done.")

    if RANK == MASTER:
        for process in range(1, SIZE):
            gathered_output_mat[(process-1) * chunkSize:] = COMM.recv(source=process, tag=3)
        endTime = MPI.Wtime()

        #print("Recv from client done.")
        #print(gathered_output_mat[252144-1])
        print("%s" % (endTime - startTime))

#@jit
def mat_mult_1d(mat1, mat1_row, mat1_col, mat2, mat2_row, mat2_col, output_mat, output_mat_row, output_mat_col):
    
    #start_time = time.time()

    for i in range(int(mat1_row)):
        for j in range(int(mat2_col)):
            var_sum = float(0.0)
            for k in range(int(mat2_row)):
                var_sum += mat1[i * mat1_col + k] * mat2[k * mat2_col + j]
            output_mat[i * output_mat_col + j] = var_sum
    #end_time = time.time()

    #print(type(output_mat[0]), "took","--- %s seconds ---" % (end_time - start_time))

if __name__ == "__main__":
    main()
