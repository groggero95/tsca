#!/usr/bin/python3
import os, sys
import subprocess
import math
import random
import gmpy2

def padhex(m,nb=32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def shift_test(sel):
	testnum = 10000
	nbit = 128
	nbit_mod = 8
	path = './main'
	operation = ['mm', 'me']

	toterr = 0

	for i in range(testnum):
		a = random.getrandbits(nbit)
		b = random.getrandbits(nbit)
		n = random.getrandbits(nbit_mod) | 1

		c = subprocess.run([path, operation[sel],padhex(a,nbit),padhex(b,nbit),padhex(n,nbit)], stdout=subprocess.PIPE)

		if sel == 0:
			res_t = (a*b*(int(gmpy2.invert(1<<nbit,n)))) % n
		elif sel == 1:
			res_t = (a >> b) & ((1<<(nbit+32))-1)


		res_m = c.stdout.decode('utf-8')

		# print("\n")
		# print(res_m)
		# print(padhex(res_t,nbit+32))
		flag = (res_t == int(res_m,16)) or (res_t == (int(res_m,16))-n)
		if not flag:
			print("\n")
			print(padhex(a,nbit))
			print(padhex(b,nbit))
			print(padhex(n,nbit))
			print(res_m)
			print(padhex(res_t,nbit+32))
			print("\n")
			toterr = toterr + 1
		sys.stdout.write("\rTest "+ operation[sel]+ ": " + str(i+1) + "/" + str(testnum) )


	print("\nTotal tested: " + str(testnum))
	print("Total errors: " + str(toterr))

if __name__ == '__main__':
	shift_test(int(sys.argv[1],10))
