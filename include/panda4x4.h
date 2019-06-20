#ifndef PANDA
#define PANDA

#define N_SAMPLES 		15000
#define MSG_FILE 		"./data/P1M_Ofast_key0_256.BIN"
#define TIME_FILE 		"./data/T1M_Ofast_key0_256.BIN"
#define B_CONSIDERED 	2
#define B_GUESSED 		1
#define FILTERING		0
#define COEFF 1.5

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
