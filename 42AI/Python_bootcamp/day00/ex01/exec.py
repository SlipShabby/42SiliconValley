import sys

if len(sys.argv) >1:
    s = ' '.join(sys.argv[1:])
    print(s[::-1].swapcase())