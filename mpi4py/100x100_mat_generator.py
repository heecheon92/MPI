def main():

    with open("100x100_matrix.txt", "w") as f:
        
        for i in range(1, 10001):
            if i % 100 == 0:
                f.write(str(i) + "\n")
            else:
                f.write(str(i) + " ")

        f.close()

if __name__ == "__main__":
    main()
