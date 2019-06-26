#
# Copyright (C) EURECOM, Telecom Paris
#
# Prof. Renaud PACALET          <renaud.pacalet@telecom-paristech.fr>
# Alberto ANSELMO               <Alberto.Anselmo@eurecom.fr>
# Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
# Fausto CHIATANTE              <Fausto.Chiatante@eurecom.fr>
# Giulio ROGGERO                <Giulio.Roggero@eurecom.fr>
#
# This file must be used under the terms of the CeCILL. This source
# file is licensed as described in the file COPYING, which you should
# have received as part of this distribution. The terms are also
# available at:
# http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt 
#

#!/usr/bin/python3
import os
import sys
import subprocess
import math
import random
import gmpy2
import matplotlib.pyplot as plt


def padhex(m, nb=32):
    """Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
    for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
    return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def mm_test(nbit=128, testnum=1000, mode=0):
    """Perform the needed tests, taking as arguments the operation (MM, ME), number of bits, number of tests, scaling coefficient for struct bits """
    pad = 128 + 32
    nbit_mod = 128
    path = './timing'
    res = []

    for i in range(testnum):
        a = random.getrandbits(nbit)  # exponent
        b = random.getrandbits(nbit)  # message
        n = random.getrandbits(nbit_mod) | 1

        k0 = pow(2,((nbit_mod+2)*2),n)

#        print(padhex(a, pad),padhex(b, pad),padhex(n, pad),padhex(k0, pad))

        c = subprocess.run([path, padhex(a, pad), padhex(
            b, pad), padhex(n, pad), padhex(k0, pad), str(mode)], stdout=subprocess.PIPE)

        res.append(int(c.stdout.decode('utf-8').strip('\n'),10))
    return res


if __name__ == '__main__':
    if len(sys.argv) == 1:
        time = mm_test()
    elif len(sys.argv) == 2:
        time = mm_test(int(sys.argv[1], 10))
    elif len(sys.argv) == 3:
        time = mm_test(int(sys.argv[1], 10), int(sys.argv[2], 10))
    elif len(sys.argv) == 4:
        time = mm_test(int(sys.argv[1], 10), int(sys.argv[2], 10), int(sys.argv[3], 10))
    #fp = open('data.txt','w')
    #for data in time:
    #	fp.write(str(data)+ '\n')
    #fp.close()
    plt.hist(time,bins=100)
    plt.ylabel("Frequency")
    plt.xlabel("Clock cycles")
    plt.show()
