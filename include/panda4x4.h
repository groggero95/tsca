#ifndef PANDA
#define PANDA

#define N_SAMPLES 		15000
#define MSG_FILE 		"./data/PLAIN.BIN"
#define TIME_FILE 		"./data/TIME.BIN"
#define B_CONSIDERED 	2
#define B_GUESSED 		1

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
