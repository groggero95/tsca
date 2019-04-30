#!/usr/bin/python3
import os, sys
import numpy
import scipy.stats as stat

class guess_test():
	def __init__(self,plain,T,n,nb):
		self.plaintext = plain
		self.n = n
		self.nb = nb
		self.T = T
		self.t = 0
		self.c = (1<<nb)%n
		self.s = (plain*(1<<nb)) % n


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

def me_step(c, s, e_bit, n,nb):
	if (e_bit):
		c = mm(c,s,n,nb)
	s = mm(s,s,n,nb)
	return c, s

def me_estimate(c, s, n, nb):
	estimate = 0
	estimate += mm_estimate(c,s,n,nb)
	estimate += mm_estimate(s,s,n,nb)

	return estimate

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

		mask = mask << 1
		# print(str(x) + ' ' + padhex(c,160) + ' ' + padhex(s,160))
	c = mm(c,1,n,nb)
	return c

def padbin(m, nb=128):
    return bin(m)[2:].zfill(nb)

def read_plain(n, nb=130, file_msg='PLAIN.BIN', file_time='TIME.BIN', length_msg=16, length_time=8):
	### The number of bytes that composes the stuff to be read
	f_msg = open(file_msg,"rb")
	messages = list()
	f_time = open(file_time, "rb")
	T_arr = list()
	while True:
		read_msg = f_msg.read(length_msg)
		read_time = f_time.read(length_time)
		if not ((read_msg) and (read_time)):
			break
		msg = guess_test(int.from_bytes(read_msg, byteorder='little',signed=False), int.from_bytes(read_time, byteorder='little',signed=False), n, nb)
		T_arr.append(msg.T)
		messages.append(msg)
	f_time.close()
	f_msg.close()
	return messages, T_arr



if __name__ == '__main__':


	#known informations
	n       = 0x0c26e8d2105e3454baf122700611e915d
	public  = 0x000000000000000000000000000010001
	private = 0x00745812bb1ffacf0b5d6200be2ced7d5
	m       = 0x00000004369616f20636f6d652076613f
	k0 	    = 0x08354f24c98cfac7a6ec8719a1b11ba4f
	nb 		= 130

	# Final to revert the key, as we start from LSB, just for testing with one bit at a time
	private_key_bit = padbin(private)[::-1]
	public_key_bit = padbin(public)[::-1]

	# private exponent to be found
	e_guess = list()

	# control
	step = 0
	tau_corr = 0.3
	tau_miss = 0.1

	# c = (1<<nb)%n
	# s = (m*(1<<nb)) % n
	messages, T_arr = read_plain(n=n, nb=nb)

	#messages[0] = guess_test(m,0,n,nb)

	for step, bit in zip(range(nb), private_key_bit):
		#calculate timing estimate based on guessing 1 for the current exponent bit
		t_arr = list()
		for test in messages:
			test.t = mm_estimate(test.c, test.s, n, nb)
			# test.t = me_estimate(test.c, test.s, n, nb)
			t_arr.append(test.t)

		# Calculate Pearson correlation coefficient
		pcc = stat.pearsonr(T_arr, t_arr)
		# Chiatant's Th : the PCC, if a correlation exists, should be larger equal than 0
		# as correlation is always as t_arr increases, T_arr does the same
		print("At step {} pcc is: {} bit: {}".format(step, pcc, bit))

		#analyze the goodness of the guess
		if (step==0):
			e_guess.append(1)
		elif (abs(pcc[0]) > tau_corr):
			e_guess.append(1)
		else:
			e_guess.append(0)

		#update known private exponent and execute next step
		for test in messages:
			test.c, test.s = me_step(test.c, test.s, int(bit), n, nb)


	# test if the step algorithm ran correctly
	c = mm(messages[0].c,1,n,nb)
	print(hex(c))
	c = me(private,messages[0].plaintext,n,nb)
	print(hex(c))
