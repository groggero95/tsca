#ifndef PANDA
#define PANDA

#define MSG_SAMPLES 9000
#define TIME_FILE "./data/T10k_Ofast_key0_128.BIN"
#define MSG_FILE "./data/P10k_Ofast_key0_128.BIN"

typedef struct msg_t
{
	bigint_t c;
	bigint_t s;
	uint32_t tot_est;
} msg_t;

#endif
