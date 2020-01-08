
def main():

    runtime = []
    
    two_processes_time = []
    two_processes_mbflops = []
    
    four_processes_time = []
    four_processes_mbflops = []
    
    eight_processes_time = []
    eight_processes_mbflops = []
    
    ten_processes_time = []
    ten_processes_mbflops = []
    
    with open("c_benchmark_512_output.txt", "r") as f:
        
        line = f.readline()
    
        while line:
            runtime.append(line.strip())
            line = f.readline()
    
        f.close()
    
            
    two_processes_time = runtime[runtime.index("Number of processes: 4")+2:]
    four_processes_time = two_processes_time[two_processes_time.index("Number of processes: 8")+2:]
    eight_processes_time = four_processes_time[four_processes_time.index("Number of processes: 1666666")+2:]
    ten_processes_time = eight_processes_time[eight_processes_time.index("Number of processes: 10")+2:]
    
    two_processes_time = two_processes_time[:two_processes_time.index("Number of processes:")]
    four_processes_time = four_processes_time[:four_processes_time.index("Number of processes:")]
    eight_processes_time = eight_processes_time[:eight_processes_time.index("Number of processes:")]

    print(two_processes_time[0], two_processes_time[len(two_processes_time) - 1])
    print(four_processes_time[0], four_processes_time[len(four_processes_time) - 1])
    print(eight_processes_time[0], eight_processes_time[len(eight_processes_time) - 1])
    print(ten_processes_time[0], ten_processes_time[len(ten_processes_time) - 1])

    print("****** RESULT in sec ******")
    print("At 2 processes: ", min(two_processes_time), " seconds")
    print("At 4 processes: ", min(four_processes_time), " seconds")
    print("At 8 processes: ", min(eight_processes_time), " seconds")
    print("At 10 processes: ", min(ten_processes_time), " seconds")
    
    print("\n")
    print("****** RESULT in mbflops ******")
    print("At 2 processes: ", getFlops(min(two_processes_time)), " mbflops")
    print("At 4 processes: ", getFlops(min(four_processes_time)), " mbflops")
    print("At 8 processes: ", getFlops(min(eight_processes_time)), " mbflops")
    print("At 10 processes: ", getFlops(min(ten_processes_time)), " mbflops")


def isFloat(string):
    try:
        float(string)
        return True
    except ValueError:
        return False

def getFlops(time):
    """
        flops = (O(n) / runtime)
        mbflops = (O(n) / runtime) * 0.000001
        
        Our naive matrix multiplication has O(n^3) complexity where n = 1000,
        for 1000x1000 matrix.
    """
    return ((1000 ** 3) / float(time)) * 0.000001

if __name__ == "__main__":
    main()
