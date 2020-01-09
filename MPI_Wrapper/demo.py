"""
Author: Heecheon Park

Description: 

1. Python program to call matmul function from C program. (Experimental)
2. Wrapping Python into MPICH.


Disclaimer:

THIS IS HIGHLY EXPERIMENTAL.

I am still in learning process as to what would be the best way to
improve the performance of matrix multiplication in Python using lists (rather numpy's ndarray)

Although numpy library provides the most efficient implementation of math and science related operations
IN Python, numpy array tends to be much slower when interacting with user-defined functions.

It is noteworthy if calling a function from C into Python elicits improvement in performance.


Personal Note:

MPI_Comm = int
MPI_Status = struct


ctypes type to Python Native type conversion.

foo = ctypes.c_int(5)

print(foo)          // c_int(5)

foo = foo.value

print(foo)          // 5
"""

import ctypes as C

class MPI_Status(C.Structure):
    _fields_ = [("count_lo", C.c_int),
                ("count_hi_and_cancelled", C.c_int),
                ("MPI_SOURCE", C.c_int),
                ("MPI_TAG", C.c_int),
                ("MPI_ERROR", C.c_int)]

"""Load my library."""
_c_pylib = C.CDLL("c_pylib.so")

"""
Some hackery for pure MPI functions that I cannot simply wrap since
some parameters are not easy to obtain in Python's context.

i.e. It is far convenient to use MPI_Datatype in the library
rather than manually defining each MPI_datatype here.
"""
_c_pylib.matmul.argtypes = (C.POINTER(C.c_double), C.c_int, C.c_int, \
                            C.POINTER(C.c_double), C.c_int, C.c_int, \
                            C.POINTER(C.c_double), C.c_int, C.c_int)

_c_pylib.mpi_matmul.argtypes = (C.POINTER(C.c_double), C.c_int, C.c_int, \
                                C.POINTER(C.c_double), C.c_int, C.c_int, \
                                C.POINTER(C.c_double), C.c_int, C.c_int)
_c_pylib.getCommunicator.restype = C.c_int
_c_pylib.getAnyStatus.restype = C.POINTER(MPI_Status)
_c_pylib.mpi_send.argtypes = (C.c_void_p, C.c_int, C.c_int, C.c_int, C.c_int)
_c_pylib.mpi_recv.argtypes = (C.c_void_p, C.c_int, C.c_int, C.c_int, C.c_int)


"""
Wrapping for pure MPI.
"""
_c_pylib.MPI_Init.argtypes = (C.POINTER(C.c_int), C.POINTER(C.POINTER(C.POINTER(C.c_char))))
_c_pylib.MPI_Comm_rank.argtypes = (C.c_int, C.POINTER(C.c_int))
_c_pylib.MPI_Comm_size.argtypes = (C.c_int, C.POINTER(C.c_int))
_c_pylib.MPI_Send.argtypes = (C.c_void_p, C.c_int, C.c_int, C.c_int, C.c_int, C.c_int)
_c_pylib.MPI_Recv.argtypes = (C.c_void_p, C.c_int, C.c_int, C.c_int, C.c_int, C.c_int, C.POINTER(MPI_Status))

_c_pylib.MPI_Init.restype = C.c_int
_c_pylib.MPI_Comm_rank.restype = C.c_int
_c_pylib.MPI_Comm_size.restype = C.c_int
_c_pylib.MPI_Send.restype = C.c_int
_c_pylib.MPI_Recv.restype = C.c_int
_c_pylib.MPI_Finalize.restype = C.c_int


MASTER = 0 

def main():

    MPI_COMM_WORLD = getCommunicator()
    MPI_STATUS = getAnyStatus()
    
    size = C.c_int
    rank = C.c_int
    
    mpi_init()

    rank = mpi_comm_rank(MPI_COMM_WORLD, rank)
    size = mpi_comm_size(MPI_COMM_WORLD, size)


    print("What is my rank?: ", rank)
    print("What is my size?: ", size)

    if (rank == 0):
        data = [float(i) for i in range(1, 10)]
    elif (rank != 0):
        data = [float(0) for i in range(1, 10)]
        print("Initial data at rank {}: {}".format(rank, data))
        

    if (rank == 0):
        for node in range(1, size):
            mpi_send(data, node, 1, MPI_COMM_WORLD)
    elif (rank != 0):
        data = mpi_recv(data, 0, 1, MPI_COMM_WORLD)
        print("Received data at {}: {}".format(rank, data))
    

    mpi_finalize()












    #mat1 = [float(i) for i in range(1, 10)]
    #mat2 = [float(i) for i in range(1, 10)]
    #output_mat = [1 for i in range(1,10)]

    #result =  matmul(mat1, 3, 3, \
    #          mat2, 3, 3, \
    #          output_mat, 3, 3)

    # Cannot "print(result)" because result is not a list
    # but a ctypes double*.
    #print(type(result))

    #for elem in result:
    #    print(elem)


    #result = mpi_matmul(mat1, 3, 3, mat2, 3, 3, output_mat, 3, 3);

    #print("Expected lenght = 9, Actual length = ", len(result))
    #for i in range(len(result)):
    #    if ((i + 1) % 3) == 0:
    #        print(result[i])
    #    else:
    #        print(result[i], end=" ")

    #print("Hello, world")


def getCommunicator():
    global _c_pylib
    return _c_pylib.getCommunicator()

def getAnyStatus():
    global _c_pylib
    return _c_pylib.getAnyStatus()

def mpi_init():
    global _c_pylib
    _c_pylib.MPI_Init(None, None)

def mpi_comm_size(COMM, SIZE):
    global _c_pylib
    SIZE = C.c_int()
    _c_pylib.MPI_Comm_size(COMM, SIZE)
    return SIZE.value

def mpi_comm_rank(COMM, RANK):
    global _c_pylib
    RANK = C.c_int()
    _c_pylib.MPI_Comm_rank(COMM, RANK)
    return RANK.value

def mpi_finalize():
    global _c_pylib
    _c_pylib.MPI_Finalize()

def mpi_send(data, destination, tag, COMM):
    global _c_pylib

    parsedData = (C.c_double * len(data))(*data)

    """
    Encode string into byte object.
    Then, convert it into string buffer that can be decayed into
    char pointer.
    """
    #parsedData = data.encode("utf-8")
    #parsedData = C.create_string_buffer(parsedData)

    _c_pylib.mpi_send(parsedData, len(data), destination, tag, COMM)

def mpi_recv(data, source, tag, COMM):
    global _c_pylib
    parsedData = (C.c_double * len(data))(*data)

    """
    Encode string into byte object.
    Then, convert it into string buffer that can be decayed into
    char pointer.
    """
    #parsedData = data.encode("utf-8")
    #parsedData = C.create_string_buffer(parsedData)

    _c_pylib.mpi_recv(parsedData, len(data), source, tag, COMM)

    parsedData = [parsedData[i] for i in range(len(data))]
    return parsedData

def matmul(matA, rowA, colA,\
           matB, rowB, colB,\
           output_mat, rowC, colC):

    global _c_pylib
    mat_size = len(output_mat)

    mat1 = (C.c_double * len(matA))(*matA)
    mat2 = (C.c_double * len(matB))(*matB)
    mat3 = (C.c_double * len(output_mat))(*output_mat)

    _c_pylib.matmul(mat1, rowA, colA, \
                    mat2, rowB, colB, \
                    mat3, rowC, colC)

    return mat3


def mpi_matmul(matA, rowA, colA,\
               matB, rowB, colB,\
               output_mat, rowC, colC):

    global _c_pylib
    mat_size = len(output_mat)

    mat1 = (C.c_double * len(matA))(*matA)
    mat2 = (C.c_double * len(matB))(*matB)
    mat3 = (C.c_double * len(output_mat))(*output_mat)

    _c_pylib.mpi_matmul(mat1, rowA, colA, \
                        mat2, rowB, colB, \
                        mat3, rowC, colC)

    return mat3

if __name__ == "__main__":
    main()
