#!/usr/bin/python3
import os
import sys
import numpy
import scipy.stats as stat
import copy
from msg_class import guess_test


def padbin(m, nb=128):
    return bin(m)[2:].zfill(nb)


def read_plain(n, nb=130, file_msg='PLAIN.BIN', file_time='TIME.BIN', length_msg=16, length_time=8):
    # The number of bytes that composes the stuff to be read
    f_msg = open(file_msg, "rb")
    messages = list()
    f_time = open(file_time, "rb")
    T_arr = list()
    i = 0
    while i < 5000:
        i = i + 1
        read_msg = f_msg.read(length_msg)
        read_time = f_time.read(length_time)
        if not ((read_msg) and (read_time)):
            break
        msg = guess_test(int.from_bytes(read_msg, byteorder='little', signed=False), int.from_bytes(
            read_time, byteorder='little', signed=False), n, nb)
        T_arr.append(msg.T)
        messages.append(msg)
    f_time.close()
    f_msg.close()
    return messages, T_arr


def step_forward(messages, bits_guessed, T_arr, tau):

    t_arr = [msg.mm_estimate() for msg in messages]
    pcc = stat.pearsonr(T_arr, t_arr)[0]
    if (len(bits_guessed) == 0):
        # Being odd, LSB is 1, always
        bits_guessed.append(1)
    elif (pcc > tau):
        # A correlation is observed, so one is guessed
        bits_guessed.append(1)
    else:
        # else we append 0
        bits_guessed.append(0)

    for msg in messages:
        msg.me_step(bits_guessed[-1])

    return pcc


def step_force(messages, bit_force):
    for msg in messages:
        msg.me_step(bit_force)
    return


def step_back(messages, bits_guessed, pcc, count=1):

    for msg in messages:
        msg.revert(count)
    # del(bits_guessed[len(bits_guessed)-count:-1])
    # del(pcc[len(pcc)-count:-1])
    return


def main_attack():

    # known informations
    n = 0x0c26e8d2105e3454baf122700611e915d
    public = 0x000000000000000000000000000010001
    private = 0x00745812bb1ffacf0b5d6200be2ced7d5
    m = 0x00000004369616f20636f6d652076613f
    k0 = 0x08354f24c98cfac7a6ec8719a1b11ba4f
    nb = 130
    nb_key = 128

    e_bits = list()

    # Read messages from file
    messages, T_arr = read_plain(
        n=n, nb=nb, file_msg='P1M_Ofast_key0_128.BIN', file_time='T1M_Ofast_key0_128.BIN')

    print("Read messages: working on {} samples".format(len(T_arr)))
    # Final to revert the key, as we start from LSB, just for testing with one bit at a time
    private_key_bit = padbin(private)[::-1]
    public_key_bit = padbin(public)[::-1]

    temp_coll = list()
    key_guessed = list()
    bits_considered = 4
    bits_guessed = 3
    step = 0

    """
	Use this parameter to select which value has to be used:
	- 4 for me_estimate
	- 3 for mm_estimate
	"""
    estimate = 4

    init_coll = [copy.deepcopy(messages) for i in range(2**bits_considered)]

    while (step < len(private_key_bit)):
        # Start new attack here
        # Loop over the possible path for the key
        for numb, branch in enumerate(init_coll):
            # bits = bin(numb)[2:].zfill(bits_considered)[::-1]
            # for b in bits:
            numb_t = numb
            for i in range(bits_considered):
                for msg in branch:
                    msg.me_estimate(numb_t & 1)
                    msg.me_step(numb_t & 1)
                numb_t >>= 1

        # time_est = [[sum([h[4] for h in msg.hist[-bits_considered::]]) for msg in branch] for branch in init_coll]
        time_est = [[msg.tot_est for msg in branch] for branch in init_coll]

        #pcc_tot = [stat.pearsonr(T_arr, t_arr)[0] if (not(numpy.isnan(stat.pearsonr(T_arr, t_arr)[0]))) else 0 for t_arr in time_est]
        pcc_tot = [stat.pearsonr(T_arr, t_arr)[0] for t_arr in time_est]
        pcc_dic = dict()
        pcc_grouped = [0 for i in range(2**(bits_guessed))]

        for i, value in enumerate(pcc_tot):
            bit_rev = bin(i)[2:].zfill(bits_considered)[::-1]
            pcc_dic[bit_rev] = value
            pcc_grouped[i % (2**bits_guessed)] += value

            #print("{}: PCC = {} ".format(bin(i)[2:].zfill(bits_considered), value))
        for k in sorted(pcc_dic.keys()):
            print("{:4}: PCC = {:7.6f} ".format(k, pcc_dic[k]))

        maxim = max(pcc_grouped)
        guess_iter = pcc_grouped.index(maxim)
        step += bits_guessed
        print(maxim, guess_iter)
        key_guessed.extend(bin(guess_iter)[2:].zfill(bits_guessed)[::-1])
        print('Step: {:>3}'.format(step))
        print(''.join(key_guessed))
        print(''.join(private_key_bit[:step]))
        error = 0
        for j in range(step):
            if private_key_bit[j] != key_guessed[j]:
                error += 1
        if (error):
            print("Houston, we have a problem -- error: {}".format(error))

        for msg in init_coll[guess_iter]:
            msg.revert(bits_considered-bits_guessed)

        msg_restart = init_coll[guess_iter].copy()
        init_coll = [copy.deepcopy(msg_restart)
                     for i in range(2**bits_considered)]

    final_key = int(''.join(map(str, key_guessed[::-1])), 2)
    print("Secret unveiled: the key is\n {}".format(hex(final_key)))

    return


if __name__ == '__main__':
    main_attack()