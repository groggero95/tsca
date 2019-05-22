#ifndef PANDA
#define PANDA

#define N_SAMPLES 		10000
#define MSG_FILE 		"./data/P20k_Ofast_key5_512.BIN"
#define TIME_FILE 		"./data/T20k_Ofast_key5_512.BIN"
#define B_CONSIDERED 	2
#define B_GUESSED 		1

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
