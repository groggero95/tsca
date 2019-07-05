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

import copy


class guess_test():
    """Class for handling the messages, with all the info related.
    - In plaintext we save the message
    - n for (p-1)*(q-1)
    - T is the time for the messages
    - t_me is the estimated time
    - c and s are the values needed for the exponentiation
    - hist is a list that stores tuples with the previous values
    - hist_len limits the length of the above mentioned list"""

    n = 0
    nb = 0
    k0 = 0

    def __init__(self, plain, T):
        self.plaintext = plain
        self.T = T
        self.t_me = 0
        self.c = self.mm_init(guess_test.k0)
        self.s = self.mm_init(guess_test.k0,plain)
        self.tot_est = 0
        self.hist = [(self.c, self.s, 0, self.t_me, self.tot_est)]
        self.hist_len = 5

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
        for i in range(guess_test.nb):
            ai = a & 1
            qi = (res + (ai & b)) & 1

            estimate += (lt[ai] & 1) + (lt[qi] & 1)
            res = (res + (lt[ai] & b) + (lt[qi] & guess_test.n))

            res = res >> 1
            a = a >> 1
        if (change):
        	if (normal):
        		self.c = res
        	else:
        		self.s = res
        return estimate

    def me_estimate(self, bit=1, change=True):
        """Estimate the time taken by the message for a complete exponentiation"""
        if bit:
            estimate = self.mm_estimate(
                True, change) + self.mm_estimate(False, change)
        else:
            estimate = self.mm_estimate(False, change)
        self.t_me = estimate
        self.tot_est += estimate
        self.hist.append((self.c, self.s, bit, estimate, self.tot_est))
        return estimate

    def mm(self, normal=True):
        """Execute the Montgomery multplication on the instance. Optional parameter to differentiate variable assignment"""
        lt = [0, ~0]
        res = 0
        if normal:
            a, b = self.c, self.s
        else:
            a, b = self.s, self.s
        for i in range(guess_test.nb):
            ai = a & 1
            qi = (res + (ai & b)) & 1
            res = (res + (lt[ai] & b) + (lt[qi] & guess_test.n))
            res = res >> 1
            a = a >> 1
        return res

    def mm_init(self,a,b=1):
        lt = [0, ~0]
        res = 0
        for i in range(guess_test.nb):
            qi = (res + (a&b)) & 1
            res = (res + (lt[a&1]&b) + (lt[qi]&guess_test.n))
            res = res >> 1
            a = a >> 1
        return res


    def revert(self, count=1):
        """Allows to backtrack to previous values (up to history length) and returns the element of the history list"""
        del(self.hist[len(self.hist)-count:])

        self.c = self.hist[-1][0]
        self.s = self.hist[-1][1]
        self.t_me = self.hist[-1][3]
        self.tot_est = self.hist[-1][4]

