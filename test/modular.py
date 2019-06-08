#!/usr/bin/python3
import os
import sys
import subprocess
import math
import random
import gmpy2

def padhex(m, nb=32):
    """Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
    for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
    return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def mm_test(sel='mm', testnum=10000, nbit=128):
    """Perform the needed tests, taking as arguments the operation (MM, ME), number of bits, number of tests, scaling coefficient for struct bits """
    # nbit = nbit + 32
    pad = nbit + 32
    path = '../main'
    operation = ['mm', 'me']
    toterr = 0

    #a = random.getrandbits(nbit)                # exponent
    #a = 0x03c71d14ee6b177683a32751c239becb0504e087ee52293d402ed6a3e01053cb
    a = 0x0745812bb1ffacf0b5d6200be2ced7d5

    if any(sel in s for s in operation):
        for i in range(testnum):
            b = random.getrandbits(nbit)            # message
            #n = random.getrandbits(nbit) | 1    # modulus
            n = 0xc26e8d2105e3454baf122700611e915d
            k0 = pow(2,((nbit+2)*2),n)

            c = subprocess.run([path, sel, padhex(a,nbit), padhex(
                b,nbit), padhex(n,nbit), padhex(k0,nbit)], stdout=subprocess.PIPE)

            res_m = c.stdout.decode('utf-8')

            if sel == 'mm':
                r = int(gmpy2.invert(1 << nbit, n))
                res_t = (a*b*r) % n
                flag = (res_t == (int(res_m, 16)%n))
                if int(res_m, 16) >= 2*n:
                    print("\n")
                    print(padhex(res_t,pad))
            elif sel == 'me':
                res_t = pow(b,a,n)
                flag = (res_t == int(res_m, 16))

            if not flag:
                print("\n")
                print(padhex(a, pad))
                print(padhex(b, pad))
                print(padhex(n, pad))
                print(padhex(k0,pad))
                print("\n")
                print(res_m.strip('\n'))
                print(padhex(res_t, pad))
                print("\n")
                toterr = toterr + 1
            sys.stdout.write("\rTest " + sel + ": " + str(i+1) + "/" + str(testnum))

        print("\nTotal tested: " + str(testnum))
        print("Total errors: " + str(toterr))
    else:
        print("Operation: " + sel + " not found. Please choose among:")
        print(operation)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        mm_test()
    elif len(sys.argv) == 2:
        mm_test(sys.argv[1])
    elif len(sys.argv) == 3:
        mm_test(sys.argv[1], int(sys.argv[2], 10))
    elif len(sys.argv) == 4:
        mm_test(sys.argv[1], int(sys.argv[2], 10),
            int(sys.argv[3], 10))
