from cs50 import get_int

while True: # simulating the equivalent of a do while loop in C to get the height of the piramid from the user
    try:
        height = get_int ("Height: ")
        if height > 0 and height < 9:
            break
    except ValueError:
        print("It's not an integer!")

for i in range(height):
    hash = i + 1
    spaces = height - i - 1
    print(f" " * spaces, "#" * hash, "  ", "#" * hash, sep="")




