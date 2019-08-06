"""
Name: Heecheon Park
Minnesota State University Moorhead

Splitting numpy matrix (1d array) and conducting

point 2 point communications among processors.

Execution Method:

mpiexec -np 3 python3 mpi_matrix_split.py
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
    receivedMat = np.random.randint(1, size=8)

    if RANK == MASTER:
        
        mat = np.random.randint(0, high=100, size=16)
        #print(mat.shape)
        print("Current processor at: {}".format(RANK))
        print("Initial matrix:")
        printMat(mat,4,4)

        print("Sending matrix to child processors!")
        COMM.Send([mat[:8], MPI.INT], dest=1, tag=1)
        COMM.Send([mat[8:], MPI.INT], dest=2, tag=2)
        #sys.stdout.flush()
        time.sleep(1)
        

    elif RANK == 1:
        print("Current processor at: {}".format(RANK))
        COMM.Recv([receivedMat, MPI.INT], source=MASTER, tag=1)
        printMat(receivedMat, 2, 4)

    elif RANK == 2:
        print("Current processor at: {}".format(RANK))
        COMM.Recv([receivedMat, MPI.INT], source=MASTER, tag=2)
        printMat(receivedMat, 2, 4)


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
