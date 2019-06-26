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
    while True:
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

    e_bits = list()

    # Read messages from file
    messages, T_arr = read_plain(n=n, nb=nb)

    # Final to revert the key, as we start from LSB, just for testing with one bit at a time
    private_key_bit = padbin(private)[::-1]
    public_key_bit = padbin(public)[::-1]

    tau_th = 0.040
    tau_low = 0.012
    i = 0
    pcc_history = list()
    err_history = list()
    count = 2
    repeat = 6

    while (i < nb):
        pcc = step_forward(messages, e_bits, T_arr, tau_th)
        pcc_history.append(pcc)
        error = 0
        for j in range(i):
            if e_bits[j] != int(private_key_bit[j]):
                error += 1
        print(private_key_bit[:i])
        s = [str(j) for j in e_bits[:i]]
        print(''.join(s))
        print("- {:3} -- PCC: {:1.6f} -- guessed: {} -- right {} -- error {}".format(i, pcc, e_bits[-1], private_key_bit[i], error))
        if pcc < tau_low:
            if err_history.count(i) < repeat:
                err_history.append(i)
                step_back(messages, e_bits, pcc_history, count)
                # count + 1
                del(e_bits[len(e_bits)-count+1:])
                e_bits[-1] = (e_bits[-1]) ^ 1
                step_force(messages, e_bits[-1])
                print("Corrected the bit at iteration {} : bits {} {}".format(
                    i, e_bits, len(e_bits)))
            else:
                print("Forced the bit at iteration {} : bits {}, {}".format(
                    i, e_bits, len(e_bits)))

        else:
            i += 1


if __name__ == '__main__':
    main_attack()
