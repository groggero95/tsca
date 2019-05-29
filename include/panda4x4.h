#ifndef PANDA
#define PANDA

#define N_SAMPLES 		20000
#define MSG_FILE 		"./data/P1M_Ofast_key4_512.BIN"
#define TIME_FILE 		"./data/T1M_Ofast_key4_512.BIN"
#define B_CONSIDERED 	3
#define B_GUESSED 		1
#define FILTERING		1

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
