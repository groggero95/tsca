#!/usr/bin/python
from ctypes import *
libmm = CDLL("./libmm.so")

a = 27
b = 52
m = 11
nb = 16
a_ = (a << 16) % m
b_ = (b << 16) % m
R = 1 << 16
#it returns addition of two numbers
varMM = libmm.MM(a_,b_,m,nb)
print "Montgomery : ", varMM

for i in range(2,R):
	#print i
	for j in range(2,R):
		i_x_j_ = (i*R*j) % m
		i_ = (i << 16) % m;
		j_ = (j << 16) % m;
		res = libmm.MM(i_,j_,m,nb);
		if res != i_x_j_:
			print "ERROR" 
	