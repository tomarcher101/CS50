import cs50

while True:
    height = 100
    print("Height: ", end ="")
    height = cs50.get_int()
    if height > 0 and height < 23:
        break

for i in range(height):
    hashNo = 2 + i
    spaceNo = height - 1 - i

    print(" " * spaceNo, end="")
    print("#" * hashNo, end="")
    print()