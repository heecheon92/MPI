"""
Name: Heecheon Park
Minnesota State University Moorhead

Exploring Broadcasting in mpi4py.

Execution Method:

mpiexec -np 3 python3 np_bcast.py
"""
from mpi4py import MPI
import numpy as np
import sys
import time

MASTER = 0

def main():
        
    COMM = MPI.COMM_WORLD
    RANK = COMM.Get_rank()
    SIZE = COMM.Get_size()
    if RANK == MASTER:
        mat = np.arange(1,17,dtype='i').reshape(4,4) #numpy array
        #mat = [i for i in range(1,17)] #python object
    else:
        #mat = np.empty(16, dtype='i').reshape(4,4) 
        mat = np.zeros((4,4), dtype='i') #numpy array 
        #mat = [0] * 16 #python object
       
    if isinstance(mat, np.ndarray):
        print("Initial matrix at processor at: {}".format(RANK))
        print(mat)
        print("Broadcasting the matrix to all processor from {}".format(MASTER))
        COMM.Bcast(mat, MASTER)
        time.sleep(1)
        if RANK != MASTER:
            print("Final matrix after the broadcast from processor: {}".format(RANK))
            print(mat)

    else:
        print("Initial matrix at processor at: {}".format(RANK))
        printMat(mat, 4, 4)
        print("Broadcasting the matrix to all processor from {}".format(MASTER))
        COMM.bcast(mat, MASTER)
        time.sleep(1)
        if RANK != MASTER:
            print("Final matrix after the broadcast from processor: {}".format(RANK))
            printMat(mat, 4, 4)

def printMat(mat, row, col):

    getDigit = lambda: len(str(max(mat)))
    maxDigit = getDigit()
    j = 0
    for i in range(1, (row*col)+1):
        print("{0:{width}}".format(mat[j], width=maxDigit) , " ", end="")
        if ((i % col) == 0) or (i == (row * col)):
            print("")
        j += 1


if __name__ == "__main__":
    main()
