#!/usr/bin/python3
import os
import sys
import numpy as np
import scipy.stats as stat
import copy
import requests
from msg_class import guess_test


def telegram_bot_sendtext(bot_message, bot_chatID):

    bot_token = '781195409:AAGIPvT3jA7P0EfUcUAHo6VVKCV1ekZCBpY'
    send_text = 'https://api.telegram.org/bot' + bot_token + '/sendMessage?chat_id=' + bot_chatID + '&parse_mode=Markdown&text=' + bot_message
    response = requests.get(send_text)
    return response.json()

def check_bot():

    if os.path.exists('./chat.txt'):
        f_chat = open('chat.txt', "r")
        for line in f_chat:
            word = line.split()
            if word[0] == os.environ.get('USER'):
                id = word[1:]
                f_chat.close()
                return True, id
        f_chat.close()
    return False, ''

def padbin(m, nb=128):
    return bin(m)[2:].zfill(nb)


def read_plain(n, nb=130, file_msg='PLAIN.BIN', file_time='TIME.BIN', length_time=8, max_messages=5000):
    # The number of bytes that composes the stuff to be read
    f_msg = open(file_msg, "rb")
    messages = list()
    f_time = open(file_time, "rb")
    T_arr = list()
    i = 0
    while i < max_messages:
        i = i + 1
        read_msg = f_msg.read((nb-2)//8)
        # print((nb-2)//8)
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
    n = 0xc26e8d2105e3454baf122700611e915d
    private = 0x0745812bb1ffacf0b5d6200be2ced7d5
    k0 = 0x8354f24c98cfac7a6ec8719a1b11ba4f
    nb = 128
    nb_key = nb + 2

    chat_on, chat_id = check_bot()

    # Read messages from file
    # m_in, T_in = read_plain(n=n, nb=nb_key, file_msg='../data/P1M_Ofast_key3_256.BIN', file_time='../data/T1M_Ofast_key3_256.BIN', max_messages=10000000)
    m_in, T_in = read_plain(n=n, nb=nb_key, file_msg='../data/P1_5M_Ofast_key0_128.BIN', file_time='../data/T1_5M_Ofast_key0_128.BIN', max_messages=10)

    # for m in m_in:
    #     print(hex(m.plaintext))
    # Evaluate the round mean and box mean, without
    t_mean = np.mean(T_in)
    t_variance = np.std(T_in)
    coeff = 0.25
    extr = 1.5
    tail = 2.8

    messages = m_in#list()
    T_arr = T_in#list()

    # for time, m in zip(T_in, m_in):
    #     if (abs(time - t_mean) > tail*t_variance):
    #     # if (extr*t_variance < abs(time - t_mean) < tail*t_variance):
    #     #if (not(t_mean - coeff*t_variance < time < t_mean + coeff*t_variance) and (abs(time - t_mean) < extr*t_variance)):
    #         T_arr.append(time)
    #         messages.append(m)

    print("Read messages: {} samples; -- After filtering {} samples".format(len(T_in),len(T_arr)), flush=True)
    if chat_on:
        sms_in = "{}, starting your test.\nRead messages: {} samples\nAfter filtering {} samples".format(chat_id[1], len(T_in),len(T_arr))
        telegram_bot_sendtext(sms_in, chat_id[0])
    # Final to revert the key, as we start from LSB, just for testing with one bit at a time
    private_key_bit = padbin(private)[::-1]
    #public_key_bit = padbin(public)[::-1]

    for m in messages:
        m.me_estimate(1)
        m.me_step(1)
        # print(m.tot_est)


    key_guessed = ['1']
    bits_considered = 2
    bits_guessed = 1
    step = 1

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

        print(time_est)

        #pcc_tot = [stat.pearsonr(T_arr, t_arr)[0] if (not(numpy.isnan(stat.pearsonr(T_arr, t_arr)[0]))) else 0 for t_arr in time_est]
        pcc_tot = [stat.pearsonr(T_arr, t_arr)[0] for t_arr in time_est]
        # pcc_tot = [stat.spearmanr(T_arr, t_arr)[0] for t_arr in time_est]
        # pcc_tot = [stat.pointbiserialr(T_arr, t_arr)[0] for t_arr in time_est]
        # pcc_tot = [stat.kendalltau(T_arr, t_arr)[0] for t_arr in time_est]
        # pcc_tot = [stat.weightedtau(T_arr, t_arr)[0] for t_arr in time_est]
        # pcc_tot = [stat.pointbiserialr(T_arr, t_arr)[0] for t_arr in time_est]
        pcc_dic = dict()
        pcc_grouped = [0 for i in range(2**(bits_guessed))]

        for i, value in enumerate(pcc_tot):
            bit_rev = bin(i)[2:].zfill(bits_considered)[::-1]
            pcc_dic[bit_rev] = value
            pcc_grouped[i % (2**bits_guessed)] += value

            #print("{}: PCC = {} ".format(bin(i)[2:].zfill(bits_considered), value))
        for k in sorted(pcc_dic.keys()):
            print("{:4}: PCC = {:7.6f} ".format(k, pcc_dic[k]), flush=True)

        maxim = max(pcc_grouped)
        guess_iter = pcc_grouped.index(maxim)

        if (nb_key - step >= bits_guessed):
            step += bits_guessed
        else:
            step = nb_key

        print("Choice: {:4} PCC: {:7.6f}".format(bin(guess_iter)[2:].zfill(bits_guessed), maxim), flush=True)
        key_guessed.extend(bin(guess_iter)[2:].zfill(bits_guessed)[::-1])
        print('Step: {:>3}'.format(step), flush=True)
        print(''.join(key_guessed), flush=True)
        print(''.join(private_key_bit[:step]), flush=True)
        error = 0
        for j in range(step):
            if private_key_bit[j] != key_guessed[j]:
                error += 1
        if (error):
            print("Houston, we have a problem -- error: {}".format(error), flush=True)

        if chat_on:
            sms = '{}: Step {:4} -> error: {:4}'.format(chat_id[1], step, error)
            telegram_bot_sendtext(sms, chat_id[0])

        for msg in init_coll[guess_iter]:
            msg.revert(bits_considered-bits_guessed)

        msg_restart = init_coll[guess_iter].copy()
        init_coll = [copy.deepcopy(msg_restart) for i in range(2**bits_considered)]

    final_key = int(''.join(map(str, key_guessed[nb_key-1::-1])), 2)
    print("Secret unveiled: the key is\n {}".format(hex(final_key)), flush=True)
    if final_key == private:
        print("We can rob some bank here, dudes")
    if chat_on:
        sms_end = 'Hey {}, we have finished!\nEnd reached {:4} bits -> error: {:4}\nThe key is:\n{}'.format(chat_id[1], step, error, hex(final_key))
        telegram_bot_sendtext(sms_end, chat_id[0])
    return


if __name__ == '__main__':
    main_attack()
