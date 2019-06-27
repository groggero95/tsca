#!/usr/bin/python3
import random
import sys

def gcd(a, b):
	while a != 0:
		a, b = b % a, a
	return b

def findModInverse(a, m):
	if gcd(a, m) != 1:
		return None
	u1, u2, u3 = 1, 0, a
	v1, v2, v3 = 0, 1, m
	while v3 != 0:
		q = u3 // v3
		v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
	return u1 % m

def rabinMiller(num):
	s = num - 1
	t = 0
   
	while s % 2 == 0:
		s = s // 2
		t += 1
	for trials in range(5):
		a = random.randrange(2, num - 1)
		v = pow(a, s, num)
		if v != 1:
			i = 0
			while v != (num - 1):
				if i == t - 1:
					return False
				else:
					i = i + 1
					v = (v ** 2) % num
		return True

def isPrime(num):
	if (num < 2):
		return False
	lowPrimes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 
	67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 
	157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 
	251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,317, 331, 337, 347, 349, 
	353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 
	457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 
	571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 
	673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 
	797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 
	911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997]
   
	if num in lowPrimes:
		return True
	for prime in lowPrimes:
		if (num % prime == 0):
			return False
	return rabinMiller(num)

def generateLargePrime(keysize):
	while True:
		num = random.randrange(2**(keysize-1), 2**(keysize))
		if isPrime(num):
			return num

def get_blinding_pair(privateKey, keysize):
	vi = random.getrandbits(keysize)
	while gcd(vi, privateKey[0]) != 1:
		vi = random.getrandbits(keysize)
	t = pow(vi,privateKey[1],privateKey[0])
	vf = findModInverse(t,privateKey[0])
	vi_M = vi * 2**(keysize+2) % privateKey[0]
	vf_M = vf * 2**(keysize+2) % privateKey[0]
	return vi_M, vf_M, vi, vf



def generateKey(keySize):
	# Step 1: Create two prime numbers, p and q. Calculate n = p * q.
	print('Generating p prime...')
	p = generateLargePrime(keySize//2)
	print('Generating q prime...')
	q = generateLargePrime(keySize//2)
	n = p * q
   
	# Step 2: Create a number e that is relatively prime to (p-1)*(q-1).
	print('Generating e that is relatively prime to (p-1)*(q-1)...')
	while True:
		e = random.randrange(2 ** (keySize - 1), 2 ** (keySize))
		if gcd(e, (p - 1) * (q - 1)) == 1:
			break

	phi = (p - 1) * (q - 1)
	# Step 3: Calculate d, the mod inverse of e.
	print('Calculating d that is mod inverse of e...')
	d = findModInverse(e, phi)
	publicKey = (n, e)
	privateKey = (n, d)
	print('Public  key = {}'.format(hex(publicKey[1])))
	print('Private key = {}'.format(hex(privateKey[1])))
	print('Modulus     = {}'.format(hex(publicKey[0])))
	k0 = pow(2, 2*(keySize+2),publicKey[0])
	print('Factor k0   = {}'.format(hex(k0)))
	vi_M, vf_M, vi, vf = get_blinding_pair(privateKey,keySize)
	print('Blinding factors:')
	print('vi = {}'.format(hex(vi)))
	print('vf = {}'.format(hex(vf)))
	print('Blinding factors, Montgomery version:')
	print('vi = {}'.format(hex(vi_M)))
	print('vf = {}'.format(hex(vf_M)))	
	return (publicKey, privateKey, k0, (vi,vf), (vi_M,vf_M))

def generateKey_silent(keySize):
	# Step 1: Create two prime numbers, p and q. Calculate n = p * q.
	p = generateLargePrime(keySize//2)
	q = generateLargePrime(keySize//2)
	n = p * q
   
	# Step 2: Create a number e that is relatively prime to (p-1)*(q-1).
	while True:
		e = random.randrange(2 ** (keySize - 1), 2 ** (keySize))
		if gcd(e, (p - 1) * (q - 1)) == 1:
			break

	phi = (p - 1) * (q - 1)
	# Step 3: Calculate d, the mod inverse of e.
	d = findModInverse(e, phi)
	publicKey = (n, e)
	privateKey = (n, d)
	k0 = pow(2, 2*(keySize+2),publicKey[0])
	vi_M, vf_M, vi, vf = get_blinding_pair(privateKey,keySize)
	return (publicKey, privateKey, k0, (vi,vf), (vi_M,vf_M))

def main(keySize = 128):
	generateKey(keySize)

if __name__ == '__main__':
	if len(sys.argv) == 1:
		main()
	else:
		main(int(sys.argv[1]))