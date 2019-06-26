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
import sys

def test_corr(tau=0.045):
	f = open("pcc.txt", "r")
	err_zero = 0
	err_one = 0
	for l in f:
		line = l.split()
		
		pcc = float(line[5][1:-1])
		step = int(line[2])
		bit = int(line[-1])
	
		if (pcc > tau) and (bit==0):
			err_zero += 1
			print("Error 0 at bit {:3} with pcc= {}".format(step, pcc))
		elif (pcc <= tau) and (bit==1):
			err_one += 1
			print("Error 1 at bit {:3} with pcc= {}".format(step, pcc))
	
	print(err_zero, err_one)

if __name__ == '__main__':
	if len(sys.argv) == 2:
		test_corr(float(sys.argv[1])/1000)
	else:
		print("As argument the tau")


