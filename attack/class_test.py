#!/usr/bin/python3
import os, sys
import numpy
import scipy.stats as stat
# def me_step(s, c )

class guess_test():
	"""Class for handling the messages, with all the info related"""
	def __init__(self,plain,T,n,nb):
		self.plaintext = plain
		self.n = n
		self.nb = nb
		self.T = T
		self.t_me = 0
		self.t_mm = 0
		self.c = (1<<nb)%n
		self.s = (plain*(1<<nb)) % n
		self.hist = list()

	def mm_estimate(self,normal=True):
		"""Estimate the time taken by the message for multplication. Optional parameter to distinguish the variable assignment"""
		lt = [0, ~0]
		res = 0
		mask = 1
		estimate = 0
		if normal == True:
			a, b = self.c, self.s
		else:
			a, b = self.s, self.s
		for i in range(self.nb):
			ai = (a & mask) >> i
			qi = (res + (ai&b)) & 1

			estimate += (lt[ai]&1) + (lt[qi]&1)
			res = (res + (lt[ai]&b) + (lt[qi]&self.n))

			res = res >> 1
			mask = mask << 1

		self.t_mm = estimate
		return estimate

	def me_estimate(self):
		"""Estimate the time taken by the message for a complete exponentiation"""
		estimate = self.mm_estimate() + self.mm_estimate(False)
		self.t_me = estimate
		return estimate

	def mm(self, normal=True):
		"""Execute the Montgomery multplication on the instance. Optional parameter to differentiate variable assignment"""
		lt = [0, ~0]
		res = 0
		mask = 1
		if normal == True:
			a, b = self.c, self.s
		else:
			a, b = self.s, self.s
		for i in range(self.nb):
			ai = (a & mask) >> i
			qi = (res + (ai&b)) & 1
			res = (res + (lt[ai]&b) + (lt[qi]&self.n))
			res = res >> 1
			mask = mask << 1
		return res

	def me_step(self, e_bit):
		"""Execute a step of the ME. Save previous step in history list, to allow backtrack"""
		if (e_bit):
			c = self.mm(True)
		else:
			c = self.c
		s = self.mm(False)
		return c, s


def me_step(c, s, e_bit, n,nb):
	if (e_bit):
		c = mm(c,s,n,nb)
	s = mm(s,s,n,nb)
	return c, s


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
	n = 0x0c26e8d2105e3454baf122700611e915d
	nb = 130
	messages, T_arr = read_plain(n=n, nb=nb)
	meth_test = list()
	func_test = list()
	for trial in messages:
		meth_test.append(trial.me_step(False))
		func_test.append(me_step(trial.c, trial.s, False, trial.n, trial.nb))

	for a, b in zip(meth_test, func_test):
		if a != b:
			print("Error: {} {}".format(a, b))
