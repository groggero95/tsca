#!/usr/bin/python3
import os, sys
import copy
#import numpy
#import scipy.stats as stat

class guess_test():
	"""Class for handling the messages, with all the info related.
	- In plaintext we save the message
	- n for (p-1)*(q-1)
	- T is the time for the messages
	- t_mm & t_me are the estimated time
	- c and s are the values needed for the exponentiation
	- hist is a list that stores tuples with the previous values
	- hist_len limits the length of the above mentioned list"""
	def __init__(self,plain,T,n,nb):
		self.plaintext = plain
		self.n = n
		self.nb = nb
		self.T = T
		self.t_me = 0
		self.t_mm = 0
		self.c = (1<<nb)%n
		self.s = (plain*(1<<nb)) % n
		self.hist = [(self.c, self.s, self.t_mm, self.t_me)]
		self.hist_len = 6

	def mm_estimate(self,normal=True, change=True):
		"""Estimate the time taken by the message for multplication. Optional parameter to distinguish the variable assignment"""
		lt = [0, ~0]
		res = 0
		mask = 1
		estimate = 0
		if (normal):
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
		if (change):
			self.t_mm = estimate
		return estimate

	def me_estimate(self):
		"""Estimate the time taken by the message for a complete exponentiation"""
		# The following to run the estimate without setting the t_mm attribute
		estimate = self.mm_estimate(True, False) + self.mm_estimate(False, False)
		self.t_me = estimate
		return estimate

	def mm(self, normal=True):
		"""Execute the Montgomery multplication on the instance. Optional parameter to differentiate variable assignment"""
		lt = [0, ~0]
		res = 0
		mask = 1
		if normal:
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

	def me_step(self, e_bit=True):
		"""Execute a step of the ME. Save previous step in history list, to allow backtrack"""
		if (e_bit):
			c = self.mm(True)
		else:
			c = self.c
		s = self.mm(False)
		if (len(self.hist) >= self.hist_len):
			self.hist.pop(0)
		self.hist.append((c, s, e_bit, self.t_mm, self.t_me))

		self.c = c
		self.s = s
		return c, s

	def revert(self, count=1):
		"""Allows to backtrack to previous values (up to history length) and returns the element of the history list"""
		self.c = self.hist[self.hist_len-1-count][0]
		self.s = self.hist[self.hist_len-1-count][1]
		self.t_mm = self.hist[self.hist_len-1-count][3]
		self.t_me = self.hist[self.hist_len-1-count][4]
		del(self.hist[self.hist_len-count:-1])
		return self.hist[self.hist_len-1-count]



def read_plain(n, nb=130, file_msg='PLAIN.BIN', file_time='TIME.BIN', length_msg=16, length_time=8):
	### The number of bytes that composes the stuff to be read
	f_msg = open(file_msg,"rb")
	messages = list()
	f_time = open(file_time, "rb")
	T_arr = list()
	count = 0
	while True:
		read_msg = f_msg.read(length_msg)
		read_time = f_time.read(length_time)
		if (not ((read_msg) and (read_time))) or (count > 5):
			break
		msg = guess_test(int.from_bytes(read_msg, byteorder='little',signed=False), int.from_bytes(read_time, byteorder='little',signed=False), n, nb)
		T_arr.append(msg.T)
		messages.append(msg)
		count += 1
	f_time.close()
	f_msg.close()
	return messages, T_arr



if __name__ == '__main__':
	n = 0x0c26e8d2105e3454baf122700611e915d
	nb = 130
	messages, T_arr = read_plain(n=n, nb=nb)
	msgc = copy.deepcopy(messages)
	meth_test = list()
	func_test = list()
	bit = 1
	bitw = bit

	for i in range(7):
		for m, mw in zip(messages, msgc):
			m.mm_estimate()
			mw.mm_estimate()
		if i == 7:
			bitw = 0
		for test, trial in zip(messages, msgc):
			# test.c, test.s = me_step(test.c, test.s, e_guess[-1], n, nb)
			test.me_step(bit)
			trial.me_step(bitw)
			#print("At {}: {} {} {} {}".format(i, test.c, trial.c, test.s, trial.s))

	for m, mw in zip(messages, msgc):
		#m.revert(2)
		#mw.revert(2)
		print(len(m.hist))
		print("{} {} {} {}".format(m.c, mw.c, m.s, mw.s))

		if ((m.c != mw.c) or (m.s != mw.s)):
			print("Error: {} {} {} {}".format(m.c, mw.c, m.s, mw.s))
