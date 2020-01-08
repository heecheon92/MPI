
def main():

    runtime = []
    
    two_processes_time = []
    two_processes_mbflops = []
    
    four_processes_time = []
    four_processes_mbflops = []
    
    eight_processes_time = []
    eight_processes_mbflops = []
    
    sixteen_processes_time = []
    sixteen_processes_mbflops = []
    
    with open("python_benchmark_512_2_output.txt", "r") as f:
        
        line = f.readline()
    
        while line:
            runtime.append(line.strip())
            line = f.readline()
    
        f.close()
    
            
    two_processes_time = runtime[runtime.index("Number of processes: 2")+2:]
    four_processes_time = two_processes_time[two_processes_time.index("Number of processes: 4")+2:]
    eight_processes_time = four_processes_time[four_processes_time.index("Number of processes: 8")+2:]
    sixteen_processes_time = eight_processes_time[eight_processes_time.index("Number of processes: 16")+2:]
    
    two_processes_time = two_processes_time[:two_processes_time.index("Number of processes: 4")]
    four_processes_time = four_processes_time[:four_processes_time.index("Number of processes: 8")]
    eight_processes_time = eight_processes_time[:eight_processes_time.index("Number of processes: 16")]


    print(two_processes_time[0], two_processes_time[len(two_processes_time) - 1])
    print(four_processes_time[0], four_processes_time[len(four_processes_time) - 1])
    print(eight_processes_time[0], eight_processes_time[len(eight_processes_time) - 1])
    print(sixteen_processes_time[0], sixteen_processes_time[len(sixteen_processes_time) - 1])

    two_processes_time = [time for time in two_processes_time if isFloat(time)]
    four_processes_time = [time for time in four_processes_time if isFloat(time)]
    eight_processes_time = [time for time in eight_processes_time if isFloat(time)]
    sixteen_processes_time = [time for time in sixteen_processes_time if isFloat(time)]
    print(two_processes_time)    

    print("****** RESULT in sec ******")
    print("At 2 processes: ", min(two_processes_time), " seconds")
    print("At 4 processes: ", min(four_processes_time), " seconds")
    print("At 8 processes: ", min(eight_processes_time), " seconds")
    print("At 16 processes: ", min(sixteen_processes_time), " seconds")
    
    print("\n")
    print("****** RESULT in mbflops ******")
    print("At 2 processes: ", getFlops(min(two_processes_time)), " mbflops")
    print("At 4 processes: ", getFlops(min(four_processes_time)), " mbflops")
    print("At 8 processes: ", getFlops(min(eight_processes_time)), " mbflops")
    print("At 16 processes: ", getFlops(min(sixteen_processes_time)), " mbflops")


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
        
        Our naive matrix multiplication has O(n^3) complexity where n = 512,
        for 512x512 matrix.
    """
    return ((512 ** 3) / float(time)) * 0.000001

if __name__ == "__main__":
    main()
