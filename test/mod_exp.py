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
import re

def padhex(m, nb=32, var_size = 32):
	"""Trasform number m into hexadecimal form, spproximating to the next 32 bit chunk.
	for example padhex(10,15) --> '0x0000000a' while padhex(10,36) --> '0x000000000000000a'"""
	return '0x' + hex(m)[2:].zfill(int(math.ceil(nb/var_size)*(var_size//4)))


def me_test(testnum=10000, nbit=128, varSize = 32):
	"""Perform the needed tests, taking as arguments the operation (MM, ME), number of bits, number of tests, scaling coefficient for struct bits """

	path = './main'
	toterr = 0
	sel = 'me'

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

	for i in range(testnum):
		a = random.getrandbits(nbit) # exponent
		b = random.getrandbits(nbit) # message
		n = random.getrandbits(nbit) | 1 # modulus
		k0 = pow(2,((effBit+2)*2),n)

		c = subprocess.run([path, sel, padhex(a,effBit,var_size), padhex(b,effBit,var_size), padhex(n,effBit,var_size), padhex(k0,effBit,var_size)], stdout=subprocess.PIPE)

		res_m = c.stdout.decode('utf-8')

		res_t = pow(b,a,n)

		if res_t != int(res_m, 16):
			print("\n")
			print("Exponent: {}".format(padhex(a, pad,var_size)))
			print("Message:  {}".format(padhex(b, pad,var_size)))
			print("Modulus:  {}".format(padhex(n, pad,var_size)))
			print("k0:       {}".format(padhex(k0,pad,var_size)))
			print("\n")
			print("Result:   {}".format(res_m.strip('\n')))
			print("Expected: {}".format(padhex(res_t, pad)))
			print("\n")
			toterr = toterr + 1
		sys.stdout.write("\rTest " + sel + ": " + str(i+1) + "/" + str(testnum))

	print("\nTotal tested: " + str(testnum))
	print("Total errors: " + str(toterr))

if __name__ == '__main__':
	if len(sys.argv) == 1:
		me_test()
	elif len(sys.argv) == 2:
		me_test(int(sys.argv[1],10))
	elif len(sys.argv) == 3:
		me_test(int(sys.argv[1], 10), int(sys.argv[2], 10))
	elif len(sys.argv) == 4:
		me_test(int(sys.argv[1], 10), int(sys.argv[2], 10), int(sys.argv[3], 10))        
	else:
		print("Wrong arguments expected: ./mod_exp.py N-Tests N-bits Var-Size")

