#!/usr/bin/python3
import os, sys
import subprocess
import math
import random

def padhex(m,nb=32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def logic_test(sel):
	testnum = 10000
	# +32 is needed to consider the additional field
	nbit = 128
	path = './main'
	operation = ['eq', 'df', 'ge', 'gt', 'le', 'lt' ]

	toterr = 0

	for i in range(testnum):
		a = random.getrandbits(nbit)
		b = random.getrandbits(nbit)
		if i < 1000:
			a = b
		c = subprocess.run([path, operation[sel],padhex(a,nbit),padhex(b,nbit)], stdout=subprocess.PIPE)

		if sel == 0:
			logic = 1 if a == b else 0
		elif sel == 1:
			logic = 1 if a != b else 0
		elif sel == 2:
			logic = 1 if a >= b else 0
		elif sel == 3:
			logic = 1 if a > b else 0
		elif sel == 4:
			logic = 1 if a <= b else 0
		elif sel == 5:
			logic = 1 if a < b else 0

		res_m = c.stdout.decode('utf-8')

		# print("\n")
		# print(res_m)
		# print(padhex(res_t,nbit+32))
		if logic != int(res_m,10):
			print("\n")
			print(padhex(a,nbit))
			print(padhex(b,nbit))
			print(res_m)
			print(logic)
			print("\n")
			toterr = toterr + 1
		sys.stdout.write("\rTest "+ operation[sel]+ ": " + str(i+1) + "/" + str(testnum))


	print("\nTotal tested: " + str(testnum))
	print("Total errors: " + str(toterr))

if __name__ == '__main__':
	logic_test(int(sys.argv[1],10))
