#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "mm.h"
#include "me.h"
#include "bigint.h"
#include "panda4x4.h"
#include "pcc.h"
#include <math.h>

#define NB INT_SIZE
#define NB_EFF NB+2

void get_filter_param(uint64_t *a, int dim, double *mean, double *std){
	double sum = 0;
	double sq_sum = 0;
	for(int i; i < dim; i++){
		sum += a[i];
		sq_sum += a[i]*a[i];
	}
	*mean = sum/dim;
	*std = sqrt(sq_sum/dim - (*mean)*(*mean));
	return;
}


int filter(uint64_t *T_in, msg_t *M_in, double mean, double std, double coeff, uint64_t *T_arr, msg_t *M_arr){
	int j = 0;
	for(int i = 0; i < N_SAMPLES; i++){
		if( fabs(T_in[i] - mean) < std*coeff ){
			memcpy(T_arr + j, T_in + i, sizeof(uint64_t));
			memcpy(M_arr + j, M_in + i, sizeof(msg_t));
			j++;
		}
	}
	return j;
}

char *int_to_bitstring_alloc(uint32_t x, int count)
{
    count = count<1 ? sizeof(x)*8 : count;
    char *pstr = malloc(count+1);
    pstr[count] = '\0';
    for(int i = count; i > 0; i--)
        pstr[count-i] = '0' | ((x >> (i-1)) & 1);
    pstr[count]=0;
    return pstr;
}

uint32_t max(double *vect, uint32_t dim){
	uint32_t index = 0;
	for(int i = 1; i < dim; i++){
		if(vect[i] > vect[index]){
			index = i;
		}
	}

	return index;
}

void read_plain(char *time_file, char * msg_file, int n_sample, uint64_t *T_arr, msg_t *M_arr, bigint_t n, bigint_t k0) {
	FILE *t_f = fopen(time_file, "rb");
	FILE *p_f = fopen(msg_file, "rb");

	bigint_t one = init(ONE);
	bigint_t m;
	m.numb[NUMB_SIZE - 1] = 0;

	for (int i = 0; i < n_sample; i++) {
		fread(T_arr + i, 8, 1, t_f);
		fread(m.numb, VAR_SIZE / 8, NUMB_SIZE - 1, p_f);
		M_arr[i].c = MM_big(k0, one, n, NB_EFF);
		M_arr[i].s = MM_big(k0, m, n, NB_EFF);
		M_arr[i].tot_est = 0;
		// print_to_stdout(&M_arr[i].s);
		// printf("\n");
		// printf("%d\n", T_arr[i]);
		// // printf("\n");
	}

}

int main(int argc, char* argv[]) {

	// Declaration of the key that we want to attack and other parameters needed for the attack
	bigint_t n = init("0xc26e8d2105e3454baf122700611e915d");
	bigint_t private = init("0x0745812bb1ffacf0b5d6200be2ced7d5");
	bigint_t k0 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f");

	// define the structure which holds timing and messages
	uint64_t *T_arr, *T_in;
	msg_t *M_arr, *M_in;
	uint32_t n_sample = N_SAMPLES;

	T_arr = (uint64_t *) malloc(sizeof(uint64_t) * n_sample);
	T_in = (uint64_t *) malloc(sizeof(uint64_t) * n_sample);
	M_arr = (msg_t *) malloc(sizeof(msg_t) * n_sample);
	M_in = (msg_t *) malloc(sizeof(msg_t) * n_sample);

	read_plain(TIME_FILE, MSG_FILE, n_sample, T_in, M_in, n, k0);

	double mean, std;
	double coeff = 2;

	printf("Post read messages\n");

	#if FILTERING == 1
		get_filter_param(T_in, n_sample, &mean, &std);

		printf("Post mean and std\n");
		n_sample = filter(T_in, M_in, mean, std, coeff, T_arr, M_arr);
	#else
		T_arr = T_in;
		M_arr = M_in;
	#endif

	printf("Prefilter %d messages, post-filter %d messages\n", N_SAMPLES, n_sample);
	uint32_t key_guessed[INT_SIZE] = {1};
	uint32_t bits_considered = B_CONSIDERED;
	uint32_t bits_guessed  = B_GUESSED;
	uint32_t step = 1;

	uint32_t window_len = (0x01 << bits_considered);
	msg_t **window = (msg_t **) malloc(sizeof(msg_t *) * window_len);

	uint32_t groups = 1 << bits_guessed;
	double *pcc_arr = (double *) malloc(sizeof(double) * groups);


	for (int i = 0; i < n_sample; i++)
	{
		ME_big_estimate(1,M_arr + i,n,1);
	}

	for (int i = 0; i < window_len; i++) {
		window[i] = (msg_t*) malloc(sizeof(msg_t) * n_sample);
		memcpy( window[i],  M_arr, sizeof(msg_t) * n_sample);
	}

	pcc_context *ctx;
	uint32_t guess;

	while (step < NB) {

		// create context for the pcc
		ctx = pcc_init(window_len);

		// Iterate over all messages in a path
		for (int i = 0; i < n_sample; i++){
			// Insert a realization of the X variable (time samples)
			pcc_insert_x(ctx, (double) T_arr[i]);
			// Iterate over all possible path
			for (int branch = 0; branch < window_len; branch++)
			{
				// advance of bits_considered bits in each message
				ME_big_estimate(branch, window[branch] + i, n, bits_considered);
				// Insert the estimate into the correct branch to have all the necessary pcc
				pcc_insert_y(ctx, branch, (double) window[branch][i].tot_est);
			}

		}
		// Compute all the pcc now
		pcc_consolidate(ctx);

		// Init the array of pcc
		for (int i = 0; i < groups; pcc_arr[i++] = 0);

		for (int i = 0; i < window_len; i++)
		{
			printf("%s :%f\n", int_to_bitstring_alloc(i,bits_considered), pcc_get_pcc(ctx,i));
		}

		// Copy and group together pcc which have bit_guessed bits in common
		for (int i = 0, j = 0; i < window_len; ++i)
		{
			pcc_arr[j] += pcc_get_pcc(ctx,i);
			if( (i + 1) % groups == 0){
				j++;
			}
		}

		// Clear the pcc for next cycle
		pcc_free(ctx);

		// Extarct the index of the max, which represent our guessed bits
		guess = max(pcc_arr,groups);

		for (int i = 0; i < n_sample; i++)
		{
			ME_big_estimate(guess,M_arr + i,n,bits_guessed);
		}

		// Advance the refernece messaged of our guess

		// Reset all possible path to the guessed state for next cycle
		for(int i = 0; i < window_len; i++) {
			memcpy( window[i],  M_arr, sizeof(msg_t) * n_sample);
		}

		printf("Guess: %x  PCC: %f\n", guess, pcc_arr[guess]);
		// Update the private key, the mask is necessary as the guess has the LSB in the left
	    int mask = (1 << (bits_guessed - 1));
		for (int i = 0; i < bits_guessed & i+step < NB; i++)
		{
			if(guess & mask){
				key_guessed[i + step] = 1;
			}else{
				key_guessed[i + step] = 0;
			}
			mask >>= 1;
		}

		step += bits_guessed;
		printf("Step: %d\n",step);

		for (int i = 0; i < step & i < NB; i++)
		{
			printf("%d", key_guessed[i]);
		}

		printf("\n");

		for (int i = 0; i < step & i < NB; i++)
		{
			printf("%d", lsr(private,i).numb[0] & 1);
		}

		printf("\n\n");


	}


	for (int i = 0; i < NB; i++) {
		if (key_guessed[i] != (lsr(private,i).numb[0] & 1)){
			printf("Houston, we have a problem: KEYS DON'T MATCH\n");
			return -1;
		}
	}
	printf("BAZINGA! Attack succeeded!\n");

	return 0;
}
