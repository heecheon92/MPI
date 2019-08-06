"""
Name: Heecheon Park
Minnesota State University Moorhead

Exploring Scatter and Gather in mpi4py with numpy array.

When dealing with memory buffer in mpi4py, a buffer parameter can optionally
be specified.

For example:

COMM.Scatter(sendbuf, recvbuf, root) is

COMM.Scatter([mat (data), 8 (count), MPI.INT (datatype)], [local_mat, 8, MPI.INT], root=MASTER)

OR

COMM.Gather(send_localbuf, recv_gatheredBuf, root) is

COMM.Gather([local_mat (data), 8 (count), MPI.INT (datatype)], [mat_gathered, 8, MPI.INT], root=MASTER)


Execution Method:

mpiexec -np 3 python3 np_scatter_and_gather.py
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
    mat = np.arange(1,25,dtype='i').reshape(6,4) #numpy array
    #mat = np.zeros((6,4), dtype='i') #numpy array
            
    local_mat = np.zeros((2,4), dtype='i') #container to receive data
    mat_gathered = np.zeros((6,4), dtype='i') #container to gather data

    if (RANK == MASTER): 
        print("Initial matrix at processor at: {}".format(RANK))
        print(mat)
        print("Scattering the matrix to all processor from {}".format(MASTER))
    #COMM.Scatter(sendbuf, recvbuf, root) is
    #COMM.Scatter([mat (data), 8 (count), MPI.INT (datatype)], [local_mat, 8, MPI.INT], root=MASTER)
    #COMM.Scatter([mat, 8, MPI.INT], local_mat, root=MASTER)
    COMM.Scatter(mat, local_mat, root=MASTER)

    print("The matrix has been scattered!")
    print("***********************************************")
    print("Local matrix at processor {}:".format(RANK))
    print(local_mat)
    print("***********************************************")
    #time.sleep(1)
    print("***********************************************")
    #COMM.Gather(send_localbuf, recv_gatheredBuf, root)
    #COMM.Gather([local_mat (data), 8 (count), MPI.INT (datatype)], [mat, 8, MPI.INT], root=MASTER)
    COMM.Gather([local_mat, 8, MPI.INT], [mat_gathered, 8, MPI.INT], root=MASTER)
    if RANK == MASTER:
        print("Gathering the local matrix to the Master Processor!")
        print("***********************************************")
        print("Gathered matrix:")
        print(mat_gathered)
    

if __name__ == "__main__":
    main()
