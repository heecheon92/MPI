from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
    data = [i for i in range(0, 16)]
    comm.send(data, dest=1, tag=11)
    #comm.Send(data, dest=1, tag=11)
    print("Data from processor ", rank, ": ",data)
elif rank == 1:
    data = comm.recv(source=0, tag=11)
    #data = comm.Recv(source=0, tag=11)
    print("Data from processor ", rank, ": ",data)
