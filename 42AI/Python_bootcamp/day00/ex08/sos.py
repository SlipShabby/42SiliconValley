import sys
import re

morse_dict = { 'A':'.-', 'B':'-...', 
                    'C':'-.-.', 'D':'-..', 'E':'.', 
                    'F':'..-.', 'G':'--.', 'H':'....', 
                    'I':'..', 'J':'.---', 'K':'-.-', 
                    'L':'.-..', 'M':'--', 'N':'-.', 
                    'O':'---', 'P':'.--.', 'Q':'--.-', 
                    'R':'.-.', 'S':'...', 'T':'-', 
                    'U':'..-', 'V':'...-', 'W':'.--', 
                    'X':'-..-', 'Y':'-.--', 'Z':'--..', 
                    '1':'.----', '2':'..---', '3':'...--', 
                    '4':'....-', '5':'.....', '6':'-....', 
                    '7':'--...', '8':'---..', '9':'----.', 
                    '0':'-----', ' ' : " / "}

def morse(s):
    def transform(c):
        c = c.upper()
        return morse_dict[c]
    return ' '.join(map(transform, s))

try:
    if len(sys.argv) > 1:
        new = [el for sub in sys.argv[1::] for el in sub.split()]
        print(' / '.join(map(morse, new)))
except KeyError:
    print('ERROR')