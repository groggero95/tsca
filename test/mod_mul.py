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
    pad = 128 + 32
    nbit_mod = 128
    path = './main'
    operation = ['mm', 'me']
    toterr = 0

    for i in range(testnum):
        a = random.getrandbits(nbit)
        b = random.getrandbits(nbit)
        n = random.getrandbits(nbit_mod) | 1
        # n = (1<<nbit)-1
        c = subprocess.run([path, operation[sel], padhex(a, pad), padhex(
            b, pad), padhex(n, pad)], stdout=subprocess.PIPE)

        r = int(gmpy2.invert(1 << nbit, n))
        if sel == 0:
            res_t = (a*b*r) % n
        elif sel == 1:
            res_t = (a >> b) & ((1 << (nbit+32))-1)

        res_m = c.stdout.decode('utf-8')

        # print("\n")
        # print(res_m)
        # print(padhex(res_t,nbit+32))
        flag = (res_t == int(res_m, 16)%n)
        if not flag:
            # print("\n")
            # print(padhex(a, pad))
            # print(padhex(b, pad))
            # print(padhex(n, pad))
            # print(res_m)
            # print(padhex(res_t, pad))
            # print("\n")
            toterr = toterr + 1
        print("\n")
        print(padhex(a, pad))
        print(padhex(b, pad))
        print(padhex(n, pad))
        print(res_m)
        print(padhex(res_t, pad))
        print("\n")
        sys.stdout.write(
            "\rTest " + operation[sel] + ": " + str(i+1) + "/" + str(testnum))

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
