def main():

    with open("1000x1000_matrix.txt", "w") as f:
        
        for i in range(1, 1000001):
            if i % 1000 == 0:
                f.write(str(i) + "\n")
            else:
                f.write(str(i) + " ")

        f.close()

if __name__ == "__main__":
    main()
