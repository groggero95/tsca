#!/usr/bin/python3
import sys

def test_corr(tau=0.045):
	f = open("pcc.txt", "r")
	err_zero = 0
	err_one = 0
	for l in f:
		line = l.split()
		
		pcc = float(line[5][1:-1])
		step = int(line[2])
		bit = int(line[-1])
	
		if (pcc > tau) and (bit==0):
			err_zero += 1
			print("Error 0 at bit {:3} with pcc= {}".format(step, pcc))
		elif (pcc <= tau) and (bit==1):
			err_one += 1
			print("Error 1 at bit {:3} with pcc= {}".format(step, pcc))
	
	print(err_zero, err_one)

if __name__ == '__main__':
	if len(sys.argv) == 2:
		test_corr(float(sys.argv[1])/1000)
	else:
		print("As argument the tau")


