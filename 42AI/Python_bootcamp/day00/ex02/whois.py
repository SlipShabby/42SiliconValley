import sys

def check_int(n):
    try:
        int(n)
        return True
    except ValueError:
        return False

if len(sys.argv) != 2 or check_int(sys.argv[1]) == False:
    print("ERROR")
elif len(sys.argv) == 2:
    if int(sys.argv[1]) == 0:
        print("I am Zero")
    elif (int(sys.argv[1]) %2) == 0:
        print("I am Even")
    else:
        print("I am Odd") 