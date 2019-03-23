#!/usr/bin/python3
import os, sys
import subprocess
import math
import random

def padhex(m,nb=32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def shift_test(sel):
	testnum = 10000
	nbit = 128 + 32
	nbit_shift = 5
	path = './main'
	operation = ['lsl', 'lsr']

	toterr = 0

	for i in range(testnum):
		a = random.getrandbits(nbit)
		b = random.getrandbits(nbit_shift)
		#b = 35
		c = subprocess.run([path, operation[sel],padhex(a,nbit),str(b)], stdout=subprocess.PIPE)

		if sel == 0:
			res_t = (a << b) & ((1<<(nbit))-1)
		elif sel == 1:
			res_t = (a >> b) & ((1<<(nbit))-1)


		res_m = c.stdout.decode('utf-8')

		# print("\n")
		# print(res_m)
		# print(padhex(res_t,nbit+32))

		if res_t != int(res_m,16):
			print("\n")
			print(padhex(a,nbit))
			print(b)
			print(res_m)
			print(padhex(res_t,nbit))
			print("\n")
			toterr = toterr + 1
		sys.stdout.write("\rTest "+ operation[sel]+ ": " + str(i+1) + "/" + str(testnum))


	print("\nTotal tested: " + str(testnum))
	print("Total errors: " + str(toterr))

if __name__ == '__main__':
	shift_test(int(sys.argv[1],10))
