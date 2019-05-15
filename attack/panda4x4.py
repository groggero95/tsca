#!/usr/bin/python3
import os
import sys
import numpy
import scipy.stats as stat
import copy
from msg_class import guess_test


def padbin(m, nb=128):
    return bin(m)[2:].zfill(nb)


def read_plain(n, nb=130, file_msg='PLAIN.BIN', file_time='TIME.BIN', length_msg=16, length_time=8, max_messages=5000):
    # The number of bytes that composes the stuff to be read
    f_msg = open(file_msg, "rb")
    messages = list()
    f_time = open(file_time, "rb")
    T_arr = list()
    i = 0
    while i < max_messages:
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



def main_attack():

    # known informations
    n = 0xc8aed04da6c85dd4638add6c6fc04a59
    private = 0x2845ecc7a890cd4356ef00ff86e63f81
    m = 0x00000004369616f20636f6d652076613f
    k0 = 0x64d8149d5c75b7137c099ce764ab8335
    nb = 130
    nb_key = 128

    # Read messages from file
    messages, T_arr = read_plain(n=n, nb=nb, file_msg='P1M_Ofast_key2_128.BIN', file_time='T1M_Ofast_key2_128.BIN', max_messages=30000)

    print("Read messages: working on {} samples".format(len(T_arr)))
    # Final to revert the key, as we start from LSB, just for testing with one bit at a time
    private_key_bit = padbin(private)[::-1]
    #public_key_bit = padbin(public)[::-1]

    key_guessed = list()
    bits_considered = 4
    bits_guessed = 2
    step = 0

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
                # To shift and pass the next LSB
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

        if (nb_key - step >= bits_guessed):
            step += bits_guessed
        else:
            step = nb_key

        print("Choice: {:4} PCC: {:7.6f}".format(bin(guess_iter)[2:].zfill(bits_guessed), maxim))
        key_guessed.extend(bin(guess_iter)[2:].zfill(bits_guessed)[::-1])
        print('Step: {:>3}'.format(step))
        print(''.join(key_guessed))
        print(''.join(private_key_bit[:step]), flush=True)
        error = 0
        for j in range(step):
            if private_key_bit[j] != key_guessed[j]:
                error += 1
        if (error):
            print("Houston, we have a problem -- error: {}".format(error), flush=True)

        for msg in init_coll[guess_iter]:
            msg.revert(bits_considered-bits_guessed)

        msg_restart = init_coll[guess_iter].copy()
        init_coll = [copy.deepcopy(msg_restart) for i in range(2**bits_considered)]

    final_key = int(''.join(map(str, key_guessed[::-1])), 2)
    print("Secret unveiled: the key is\n {}".format(hex(final_key)))

    return


if __name__ == '__main__':
    main_attack()
