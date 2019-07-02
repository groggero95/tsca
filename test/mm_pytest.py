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
import gmpy2

def mm(a,b,n,nb):
	print(a,b,n,nb)
	step = []
	lt = [0, ~0]
	res = 0
	for i in range(nb):
		qi = (res + (a&b)) & 1
		res = (res + (lt[a&1]&b) + (lt[qi]&n))
		res = res >> 1
		step.append( str(i) + ' ' +  str(a&1) + ' ' + str(qi) + ' ' + hex(res))
		a = a >> 1
	return res, step


def test(nb):
	n = (1<<nb) - 1
	r = gmpy2.invert(1<<nb,n)
	for i in range(n):
		for j in range(n):
			a = mm(i,j,n,nb) % n
			b = (i*j*r) % n
			if a != b:
				print(i,j,a,b)

def diff_test(a,b,n,pad=128):
	control = mm(a,b,n,pad)
	


if __name__ == '__main__':
	test(int(sys.argv[1],10))

