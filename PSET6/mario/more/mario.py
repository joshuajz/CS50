from cs50 import get_int

# Loops until we get correct input
while True:
    height = get_int("Height: ")
    if (1 <= height and height <= 8):
        break

# Variables
spaces = height - 1
boxes = 1

# Loops
for i in range(height):
    print(" " * spaces + "#" * boxes + "  " + boxes * "#", end="")
    spaces -= 1
    boxes += 1
    print()