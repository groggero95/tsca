#!/usr/bin/python3
import os, sys
import gmpy2

def mm(a,b,n,nb):
	step = []
	lt = [0, ~0]
	res = 0
	mask = 1
	for i in range(nb):
		ai = (a & mask) >> i
		qi = (res + (ai&b)) & 1
		res = (res + (lt[ai]&b) + (lt[qi]&n))
#		print(res)
		res = res >> 1
		mask = mask << 1
		step.append( str(i) + ' ' +  str(ai) + ' ' + str(qi) + ' ' + str(padhex(res,128+32)))
# only used for testing pourposes
#	if res >= n:
#		return res - n
	return step


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



0x000000008c604d550243f8c63f8fcf89bee3403d
0x000000002eb9aac6008f409dce00417af10b82c0
0x000000004ce75850386df85120c62ccc109030df


0x000000001eca68c5696481c8ea5cdb9fcc56c09f
0x00000000010157b8e7fd64ac3cfbc405863e3412
0x000000004135b3a6fe7c5ad8d6ed4741e3fcb6ab

