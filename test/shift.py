#!/usr/bin/python3
import os, sys
import subprocess
import math
import random

def padhex(m,nb=32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/32)*8))


def shift_test(sel='lsl',testnum=10000,nbit=128):
	# nbit = nbit + 32
	pad = nbit + 32
	nbit_shift = 7
	path = '../main'
	operation = ['lsl', 'lsr']
	toterr = 0

	if any(sel in s for s in operation):
		for i in range(testnum):
			a = random.getrandbits(nbit) & ((1<<(nbit))-1)
			# b = random.getrandbits(nbit_shift)
			b = 16
			c = subprocess.run([path,sel,padhex(a,nbit),str(b)], stdout=subprocess.PIPE)

			if sel == 'lsl':
				res_t = (a << b) & ((1<<(pad))-1)
			elif sel == 'lsr':
				res_t = (a >> b) & ((1<<(pad))-1)

			res_m = c.stdout.decode('utf-8')

			if res_t != int(res_m,16):
				print("\n")
				print(padhex(a,pad))
				print(b)
				print(res_m)
				print(padhex(res_t,pad))
				print("\n")
				toterr = toterr + 1
			sys.stdout.write("\rTest "+ sel + ": " + str(i+1) + "/" + str(testnum))

		print("\nTotal tested: " + str(testnum))
		print("Total errors: " + str(toterr))

	else:
		print("Operation: " + sel + " not found. Please choose among:")
		print(operation)

if __name__ == '__main__':
	if len(sys.argv) == 1:
		shift_test()
	elif len(sys.argv) == 2:
		shift_test(sys.argv[1])
	elif len(sys.argv) == 3:
		shift_test(sys.argv[1], int(sys.argv[2], 10))
	elif len(sys.argv) == 4:
		shift_test(sys.argv[1], int(sys.argv[2], 10), int(sys.argv[3], 10))
