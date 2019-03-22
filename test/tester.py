#!/usr/bin/python3
import os, sys
import subprocess
import math
import random

def padhex(m,nb=32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


testnum = 1000
nbit = 128
path = './path'
operation = ['sum', 'sub', 'mul']

for i in range(testnum):
	a = random.genrandbit(nbit)
	b = random.genrandbit(nbit)
	c = subprocess.run([path_to_progr, operation[0],padhex(a,nbit),padhex(b,nbit)], stdout=subprocess.PIPE)
	for line in c.stdout.decode('utf-8').split("\n"):
		print line

