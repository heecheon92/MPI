#!/usr/bin/env python3

import sys
import subprocess


def main():


    with open("python_benchmark_512_2_output.txt", "a") as f:
        f.write("Benchmark result in MPICH.\n\n")
        number_of_processes = [2, 4, 6, 8, 16]
        for process in number_of_processes:
            if process == 2:
                print("Number of processes: " + str(process))
                f.write("Number of processes: " + str(process) + "\n\n")
                for i in range(100):
                    cmd = "mpirun.mpich -np 3 --host master:1,client:2 python3 mpi_matmul_sr_1dList_512.py"
                    time_taken = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, universal_newlines=True).communicate()[0].rstrip()
                    time_taken = time_taken.split("\n")
                    for each_time in time_taken:
                        f.write(each_time + "\n")
            if process == 4:
                print("Number of processes: " + str(process))
                f.write("Number of processes: " + str(process) + "\n\n")
                for i in range(100):
                    cmd = "mpirun.mpich -np 5 --host master:2,client:1,client2:1,client3:1 python3 mpi_matmul_sr_1dList_512.py"
                    time_taken = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, universal_newlines=True).communicate()[0].rstrip()
                    time_taken = time_taken.split("\n")
                    for each_time in time_taken:
                        f.write(each_time + "\n")
            elif process == 8:
                print("Number of processes: " + str(process))
                f.write("Number of processes: " + str(process) + "\n\n")
                for i in range(100):
                    cmd = "mpirun.mpich -np 9 --host master:2,client:1,client2:1,client3:1,client4:1,client5:1,client6:1,client7:1 python3 mpi_matmul_sr_1dList_512.py"
                    time_taken = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, universal_newlines=True).communicate()[0].rstrip()
                    time_taken = time_taken.split("\n")
                    for each_time in time_taken:
                        f.write(each_time + "\n")
            elif process == 16:
                print("Number of processes: " + str(process))
                f.write("Number of processes: " + str(process) + "\n\n")
                for i in range(100):
                    cmd = "mpirun.mpich -np 17 --host master:3,client:2,client2:2,client3:2,client4:2,client5:2,client6:2,client7:2 python3 mpi_matmul_sr_1dList_512.py"
                    time_taken = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, universal_newlines=True).communicate()[0].rstrip()
                    time_taken = time_taken.split("\n")
                    for each_time in time_taken:
                        f.write(each_time + "\n")
            
    f.close()



if __name__ == "__main__":
    main()
