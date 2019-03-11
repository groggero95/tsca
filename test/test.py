#!/usr/bin/python

# This is right, even if we are doing modulus
# operation the output res is not strictly less than
# the modulo n, thus we can have outcomes like 40
# when doing modulo 29 operations. We dont care right 
# now, the important part to remember is that the
# output res is bound to be : res < 2n
# other conditions:
# a,b < 2n
# 2n < 2**(nb-1) ???

# a : operand 1
# b : operand 2
# n : modulus
# nb: bit number
def mm(a,b,n,nb):
	lt = [0, ~0]
	res = 0
	mask = 1
	for i in range(nb):
		ai = (a & mask) >> i
		qi = (res + (ai&b)) & 1
		res = (res + (lt[ai]&b) + (lt[qi]&n)) >> 1
		mask = mask << 1
#		print i, ai, qi, res, lt
# only used for testing pourposes
#	if res >= n:
#		return res - n
	return res


# e : exponent
# m : message
# n : modulus
# nb: bit number
def me(e,m,n,nb):
# At some point we will settle up to a fixed number of bits
# so the factor (2**(2nb) mod n) can actually be precomputed
# once the public and the private key are decided  we still
# need to find a way of compuing that
#	c = mm(1, (2**(2nb) mod n),n,nb)
#	s = mm(m, (2**(2nb) mod n),n,nb)
	c = (1<<nb) % n
	s = m*(1<<nb) % n
	mask = 1
	for x in range(nb):
		if (e & mask):
			c = mm(c,s,n,nb)
		s = mm(s,s,n,nb)
		mask = mask << 1
		print x, c, s
	c = mm(c,1,n,nb)
	return c
