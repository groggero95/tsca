#!/usr/bin/python3

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

import os, sys
import subprocess
import math
import random
import re

def padhex(m, nb=32, var_size = 32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/var_size)*(var_size//4)))


def comp_test(sel='eq', testnum=10000, nbit=128, varSize = 32):

	path = './main'
	operation = ['eq', 'df', 'ge', 'gt', 'le', 'lt' ]
	toterr = 0

	curDir = os.getcwd()

	x = re.search("/tsca/?",curDir)
	if x:
		os.chdir(curDir[:x.end()])
	else:
		print("Must be inside the repository to launch the test")
		sys.exit(-1)

	curBit = int(subprocess.run(["grep", "-oP", r"^#\w+ INT\w+ \K[0-9]+", "./include/bigint.h"], stdout=subprocess.PIPE).stdout.decode('utf-8')[:-1],10)
	var_size = int(subprocess.run(["grep", "-oP", r"^#\w+ VAR\w+ \K[0-9]+", "./include/bigint.h"], stdout=subprocess.PIPE).stdout.decode('utf-8')[:-1],10)

	effBit = int(pow(2,math.ceil(math.log(nbit,2))))

	flag_bit = curBit != effBit
	flag_size = var_size != varSize

	if flag_size:
		if varSize != 32 and varSize != 64:
			print("Only 32 and 64 bit variable are supported")
			sys.exit(-1)
		os.system('sed -ri ' + r"'s/(^#\w+ VAR\w+) [0-9]+/\1 {}/g'".format(varSize) + " ./include/bigint.h")
		var_size = varSize

	if flag_bit:
		os.system('sed -ri ' + r"'s/(^#\w+ INT\w+) [0-9]+/\1 {}/g'".format(effBit) + " ./include/bigint.h")

	if (not ('main' in os.listdir())) or flag_bit or flag_size:
		subprocess.run(["make", "main"], stdout=subprocess.PIPE)

	pad = effBit + var_size

	if any(sel in s for s in operation):
		for i in range(testnum):
			a = random.getrandbits(nbit)
			b = random.getrandbits(nbit)
			if i < int(testnum/10):
				a = b
			c = subprocess.run([path,sel,padhex(a,pad,var_size),padhex(b,pad,var_size)], stdout=subprocess.PIPE)

			if sel == 'eq':
				logic = 1 if a == b else 0
			elif sel == 'df':
				logic = 1 if a != b else 0
			elif sel == 'ge':
				logic = 1 if a >= b else 0
			elif sel == 'gt':
				logic = 1 if a > b else 0
			elif sel == 'le':
				logic = 1 if a <= b else 0
			elif sel == 'lt':
				logic = 1 if a < b else 0

			res_m = c.stdout.decode('utf-8')

			if logic != int(res_m,10):
				print("\n")
				print(padhex(a,pad,var_size))
				print(padhex(b,pad,var_size))
				print(res_m)
				print(logic)
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
		comp_test()
	elif len(sys.argv) == 2:
		comp_test(sys.argv[1])
	elif len(sys.argv) == 3:
		comp_test(sys.argv[1], int(sys.argv[2], 10))
	elif len(sys.argv) == 4:
		comp_test(sys.argv[1], int(sys.argv[2], 10), int(sys.argv[3], 10))
	elif len(sys.argv) == 5:
		comp_test(sys.argv[1], int(sys.argv[2], 10), int(sys.argv[3], 10), int(sys.argv[4], 10))
	else:
		print("Wrong arguments expected: ./comp.py Test-Type N-Tests N-bits Var-Size")