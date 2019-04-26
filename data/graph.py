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

    f_time = open(time,"rb")
    while True:
        measure = f_time.read(8)
        if not measure:
            break
        timing.append(int.from_bytes(measure, byteorder='little',signed=False))

    sigma = numpy.std(timing)
    mu = numpy.mean(timing)
    print(len(timing))
    print(sigma)
    print(mu)
    plt.hist(timing,bins=1000)
    plt.ylabel("Frequency")
    plt.xlabel("Clock cycles")
    plt.show()
