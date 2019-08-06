"""
Name: Heecheon Park
Minnesota State University Moorhead

Exploring mpi4py with different datatypes and functions.

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
    receivedMat = np.empty(8, dtype=np.int64)
    #receivedMat2 = np.empty(8, dtype=np.int64)
    #receivedList = [0] * 8
    if RANK == MASTER:
        
        tmpMat = [i for i in range(0,16)]
        mat = np.array(tmpMat)

        print("Current processor at: {}".format(RANK))
        print("Initial matrix:")
        print(mat)

        print("Sending matrix to child processors!")
        COMM.Send([mat[:8], MPI.INT], dest=1, tag=1)
        COMM.Send([mat[8:], MPI.INT], dest=2, tag=2)
        #COMM.send([mat[:8], MPI.INT], dest=1, tag=1)
        #COMM.send([mat[8:], MPI.INT], dest=2, tag=2)
        time.sleep(1)
        

    elif RANK == 1:
        print("Current processor at: {}".format(RANK))
        COMM.Recv([receivedMat, MPI.INT], source=MASTER, tag=1)
        #COMM.recv([receivedList, MPI.INT], source=MASTER, tag=1)
        print("Received matrix:", receivedMat)

    elif RANK == 2:
        print("Current processor at: {}".format(RANK))
        COMM.Recv([receivedMat, MPI.INT], source=MASTER, tag=2)
        #COMM.recv([receivedList, MPI.INT], source=MASTER, tag=2)
        print("Received matrix:", receivedMat)


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
