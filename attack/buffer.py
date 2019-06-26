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

#!/usr/bin/python3
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
		if i == 5:
			bitw = 0
		for test, trial in zip(messages, msgc):
			# test.c, test.s = me_step(test.c, test.s, e_guess[-1], n, nb)
			test.me_step(bit)
			trial.me_step(bitw)
			#print("At {}: {} {} {} {}".format(i, test.c, trial.c, test.s, trial.s))

	for m, mw in zip(messages, msgc):
		m.revert(2)
		mw.revert(2)
		print(len(m.hist))
		print("{} {} {} {}".format(m.c, mw.c, m.s, mw.s))

		if ((m.c != mw.c) or (m.s != mw.s)):
			print("Error: {} {} {} {}".format(m.c, mw.c, m.s, mw.s))
