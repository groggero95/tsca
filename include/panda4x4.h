#ifndef PANDA
#define PANDA

#define N_SAMPLES 		10000
#define MSG_FILE 		"./data/P1M_Ofast_key0_128.BIN"
#define TIME_FILE 		"./data/T1M_Ofast_key0_128.BIN"
#define B_CONSIDERED 	2
#define B_GUESSED 		1
#define ATTACK_MUL		1
#define ATTACK_SQUARE	1
#define FILTERING		0
#define COEFF 			3

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
