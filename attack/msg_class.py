#!/usr/bin/python3
import copy

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
		self.tot_est = 0
		self.hist = [(self.c, self.s, 1, self.t_mm, self.t_me, self.tot_est)]
		self.hist_len = 6

	def mm_estimate(self, normal=True, change=True):
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
			ai = a & 1
			qi = (res + (ai&b)) & 1

			estimate += (lt[ai]&1) + (lt[qi]&1)
			res = (res + (lt[ai]&b) + (lt[qi]&self.n))

			res = res >> 1
			a = a >> 1
		if (change):
			self.t_mm = estimate
		return estimate

	def me_estimate(self, bit=1):
		"""Estimate the time taken by the message for a complete exponentiation"""
		# The following to run the estimate without setting the t_mm attribute
		if bit:
			estimate = self.mm_estimate(True, False) + self.mm_estimate(False, False)
		else:
			estimate = self.mm_estimate(False, False)
		self.t_me = estimate
		self.tot_est += estimate
		return estimate

	def mm(self, normal=True):
		"""Execute the Montgomery multplication on the instance. Optional parameter to differentiate variable assignment"""
		lt = [0, ~0]
		res = 0
		if normal:
			a, b = self.c, self.s
		else:
			a, b = self.s, self.s
		for i in range(self.nb):
			ai = a & 1
			qi = (res + (ai&b)) & 1
			res = (res + (lt[ai]&b) + (lt[qi]&self.n))
			res = res >> 1
			a = a >> 1
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
		self.hist.append((c, s, e_bit, self.t_mm, self.t_me, self.tot_est))

		self.c = c
		self.s = s
		return c, s

	def revert(self, count=1):
		"""Allows to backtrack to previous values (up to history length) and returns the element of the history list"""
		del(self.hist[len(self.hist)-count:])
		self.c = self.hist[-1][0]
		self.s = self.hist[-1][1]
		self.t_mm = self.hist[-1][3]
		self.t_me = self.hist[-1][4]
		self.tot_est = self.hist[-1][5]

		return self.hist[-1]
