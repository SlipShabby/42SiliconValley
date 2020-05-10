import sys
import string
import re

if len(sys.argv) != 3 or not sys.argv[2].isdigit() or sys.argv[1].isdigit():
    print('ERROR')
else:
    n = int(sys.argv[2])
    splitted = r'[{} ]'.format(string.punctuation)
    print([i for i in re.split(splitted, sys.argv[1]) if len(i) > n])