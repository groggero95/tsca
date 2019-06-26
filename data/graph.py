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
import struct
import matplotlib.pyplot as plt
import numpy


plain = "./PLAIN.BIN"
time = "./TIME.BIN"

timing = []

if __name__ == '__main__':

    if len(sys.argv) > 1:
        time = sys.argv[1]

    f_time = open(time,"rb")
    while True:
        measure = f_time.read(8)
        if not measure:
            break
        timing.append(int.from_bytes(measure, byteorder='little',signed=False)/10000)

    sigma = numpy.std(timing)
    mu = numpy.mean(timing)
    print(len(timing))
    print(sigma)
    print(mu)
    plt.hist(timing,bins=1000)
    plt.ylabel("Frequency")
    plt.xlabel("Clock cycles")
    plt.show()
