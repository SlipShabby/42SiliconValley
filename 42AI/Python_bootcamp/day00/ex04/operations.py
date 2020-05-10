import sys

usage = "Usage: python operations.py\nExample:\n\tpython operations.py 10 3"

def operations(a, b):
    return (
        a + b,
        a - b, 
        a * b,
        a / b if b != 0 else 'ERROR (div by zero)',
        a % b if b != 0 else 'ERROR (modulo by zero)'
    )

if len(sys.argv) > 3:
    print("InputError: too many arguments")
    print(usage)
elif len(sys.argv) < 3:
    print(usage)
else:
    try:
        a = int(sys.argv[1])
        b = int(sys.argv[2])
        s,d,m,dv,r = operations(a,b)

        print('Sum:       ', s)
        print('Difference:', d)
        print('Product:   ', m)
        print('Quotient:  ', dv)
        print('Remainder: ', r)
    except ValueError:
        print('InputError: only numbers')
        print(usage)
    
