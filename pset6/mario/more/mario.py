import cs50


def main():
    n = get_height_int()
    for i in range(n):
        for j in range(n + 3 + i):
            if ((j >= (n - 1) - i) and (j <= (n - 1))) or (j >= n + 2):
                print("#", end="")
            else:
                print(" ", end="")
        print()


def get_height_int():
    while True:
        n = cs50.get_int("Height: ")
        if n > 0 and n < 9:
            break
    return n


main()