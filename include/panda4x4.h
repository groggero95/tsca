#ifndef PANDA
#define PANDA

#define N_SAMPLES 		100000
#define MSG_FILE 		"./data/P100k_Ofast_key0_blind_128.BIN"
#define TIME_FILE 		"./data/T100k_Ofast_key0_blind_128.BIN"
#define B_CONSIDERED 	2
#define B_GUESSED 		1
#define FILTERING		0

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
