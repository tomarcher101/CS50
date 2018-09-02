import cs50

height = 0

while height <= 0 or height >= 24:
    height = cs50.get_int("Height: ")

for i in range(height):
    no_of_hash = 2 + i
    no_of_space = height - 1 - i
    space_left = no_of_space
    hash_left = no_of_hash

    while space_left > 0:
        print(" ", end="")
        space_left -= 1

    while hash_left > 0:
        print("#", end="")
        hash_left -= 1

    print('')
