#!/usr/bin/python3
import os, sys
# def me_step(s, c )

def mm(a,b,n,nb):
	lt = [0, ~0]
	res = 0
	mask = 1
	for i in range(nb):
		ai = (a & mask) >> i
		qi = (res + (ai&b)) & 1
		res = (res + (lt[ai]&b) + (lt[qi]&n))

		res = res >> 1
		mask = mask << 1
	return res

def mm_estimate(a,b,n,nb):
	lt = [0, ~0]
	res = 0
	mask = 1
	estimate = 0
	for i in range(nb):
		ai = (a & mask) >> i
		qi = (res + (ai&b)) & 1

		estimate += (lt[ai]&1) + (lt[qi]&1)
		res = (res + (lt[ai]&b) + (lt[qi]&n))

		res = res >> 1
		mask = mask << 1

	return estimate

def me_step(c, s, e, n, step):
	mask = 1 << step
	if (e & mask):
		c = mm(c,s,n,nb)
	s = mm(s,s,n,nb)

	return c,s

def me(e,m,n,nb):
# At some point we will settle up to a fixed number of bits
# so the factor (2**(2nb) mod n) can actually be precomputed
# once the public and the private key are decided  we still
# need to find a way of compuing that
#	c = mm(1, (2**(2nb) mod n),n,nb)
#	s = mm(m, (2**(2nb) mod n),n,nb)
#	c = mm(1,k0,n,nb)
#	s = mm(m,k0,n,nb)
	c = (1<<nb)%n
	s = (m*(1<<nb)) % n
	# print(padhex(c,160))
	# print(padhex(s,160))
	mask = 1
	for x in range(nb):
		if (e & mask):
			c = mm(c,s,n,nb)
		s = mm(s,s,n,nb)
		if (c>n or s>n):
			print("errore overflow")
		mask = mask << 1
		# print(str(x) + ' ' + padhex(c,160) + ' ' + padhex(s,160))
	c = mm(c,1,n,nb)
	return c

if __name__ == '__main__':

	#known informations
	n       = 0x0e9d83cb9ebf9f88a17f569dcb0945be1
	public  = 0x000000000000000000000000000010001
	private = 0x02ca5e5ca55dcef7008e7656da6ab2801
	m       = 0x000000000882beb97c2eb4916c19d16f3
	k0 	    = 0x08354f24c98cfac7a6ec8719a1b11ba4f
	nb 		= 130

	#private exponent to be found
	e = 0

	#control
	step = 0
	t = 0

	c = (1<<nb)%n
	s = (m*(1<<nb)) % n
	#print(padhex(c,160))
	#print(padhex(s,160))

	for step in range(nb):
		#calculate timing estimate based on guessing 1 for the current exponent bit
		t = mm_estimate(c,s,n,nb)


		#calculate pearson correlation coefficient


		#analyze the goodness of the guess


		#update known private exponent and execute next step
		c, s = me_step(c,s,public,n,step)

	c = mm(c,1,n,nb)
	print(hex(c))
	c = me(public,m,n,nb)
	print(hex(c))
