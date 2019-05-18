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


def mm_test(sel=0, nbit=128, testnum=1000, over=1):
    """Perform the needed tests, taking as arguments the operation (MM, ME), number of bits, number of tests, scaling coefficient for struct bits """
    pad = nbit + 32
    nbit_mod = nbit
    path = './main'
    operation = ['mm', 'me']
    toterr = 0

    a = random.getrandbits(nbit)  # exponent

    a = 0x03c71d14ee6b177683a32751c239becb0504e087ee52293d402ed6a3e01053cb

    for i in range(testnum):
        b = random.getrandbits(nbit)  # message
#        n=0
#        while (n < b):
        n = random.getrandbits(nbit_mod) | 1
        #n = (1<<nbit)-1

        k0 = pow(2,((nbit_mod+2)*2),n)

        c = subprocess.run([path, operation[sel], padhex(a, pad), padhex(
            b, pad), padhex(n, pad), padhex(k0, pad)], stdout=subprocess.PIPE)

        res_m = c.stdout.decode('utf-8')

        if sel == 0:
            r = int(gmpy2.invert(1 << nbit_mod, n))
            res_t = (a*b*r) % n
            flag = (res_t == (int(res_m, 16)%n))
            if int(res_m, 16) >= 2*n:
                print("\n")
                print(padhex(res_t,pad))
        elif sel == 1:
            res_t = pow(b,a,n)
            flag = (res_t == int(res_m, 16))


        if not flag:
            print("\n")
            print(padhex(b, pad))
            print(padhex(a, pad))
            print(padhex(n, pad))
            print("\n")
            print(res_m.strip('\n'))
            print(padhex(res_t, pad))
            print(padhex(k0,pad))
            print("\n")
            toterr = toterr + 1
        sys.stdout.write("\rTest " + operation[sel] + ": " + str(i+1) + "/" + str(testnum))

    print("\nTotal tested: " + str(testnum))
    print("Total errors: " + str(toterr))


if __name__ == '__main__':
    if len(sys.argv) == 1:
        mm_test()
    elif len(sys.argv) == 2:
        mm_test(int(sys.argv[1], 10))
    elif len(sys.argv) == 3:
        mm_test(int(sys.argv[1], 10), int(sys.argv[2], 10))
    elif len(sys.argv) == 4:
        mm_test(int(sys.argv[1], 10), int(
            sys.argv[2], 10), int(sys.argv[3], 10))
    elif len(sys.argv) == 5:
        mm_test(int(sys.argv[1], 10), int(sys.argv[2], 10),
                int(sys.argv[3], 10), int(sys.argv[4], 10))
