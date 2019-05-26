#ifndef PANDA
#define PANDA

#define N_SAMPLES 		10000
#define MSG_FILE 		"./data/P10k_Ofast_key0_128.BIN"
#define TIME_FILE 		"./data/T10k_Ofast_key0_128.BIN"
#define B_CONSIDERED 	4
#define B_GUESSED 		2
#define FILTERING		1

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
