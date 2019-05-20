#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "mm.h"
#include "me.h"
#include "bigint.h"
#include "panda4x4.h"
#include "pcc.h"

#define NB INT_SIZE
#define NB_EFF NB+2


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
		// print_to_stdout(&m);
		// printf("\n");
		M_arr[i].c = MM_big(k0, one, n, NB_EFF);
		M_arr[i].s = MM_big(k0, m, n, NB_EFF);
		M_arr[i].tot_est = 0;
	}

}

int main(int argc, char* argv[]) {

	// Declaration of the key that we want ro attack and other parameters needed for the attack
	bigint_t n = init("0xc26e8d2105e3454baf122700611e915d");
	bigint_t private = init("0x0745812bb1ffacf0b5d6200be2ced7d5");
	bigint_t k0 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f");

	// define the structure which holds timing and messages
	uint64_t *T_arr;
	msg_t *M_arr;
	uint32_t n_sample = 10;

	T_arr = (uint64_t *) malloc(sizeof(uint64_t) * n_sample);
	M_arr = (msg_t *) malloc(sizeof(msg_t) * n_sample);

	read_plain("../tsca/data/T1_5M_Ofast_key0_128.BIN", "../tsca/data/P1_5M_Ofast_key0_128.BIN", n_sample, T_arr, M_arr, n, k0);


	uint32_t key_guessed[INT_SIZE] = {0};
	uint32_t bits_considered = 3;
	uint32_t bits_guessed  = 1;
	uint32_t step = 1;

	uint32_t window_len = (0x01 << bits_considered);
	msg_t **window = (msg_t **) malloc(sizeof(msg_t *) * window_len);

	uint32_t groups = 1 << bits_guessed;
	double *pcc_arr = (double *) malloc(sizeof(double) * groups);


	for (int i = 0; i < window_len; i++) {
		window[i] = (msg_t*) malloc(sizeof(msg_t) * n_sample);
		// printf("%x %x\n", M_arr, window[i]);
		memcpy( window[i],  M_arr, sizeof(msg_t) * n_sample);
	}

	ME_big_estimate(1,M_arr,n,1);
	// pcc_context *ctx_ref = pcc_init(window_len);

	// for(int i = 0; i < n_sample; i++)
	// {
	// 	pcc_insert_x(cnt_ref, (double) T_arr[i]);
	// }

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
				printf("%d\n", window[branch][i].tot_est);
			}
			
		}

		// Compute all the pcc now
		pcc_consolidate(ctx);

		// Init the array of pcc
		for (int i = 0; i < groups; pcc_arr[i++] = 0);

		for (int i = 0; i < window_len; ++i)
		{
			printf("%f\n", pcc_get_pcc(ctx,i));
		}

		// Copy and group together pcc which have bit_guessed bits in common
		for (int i = 0, j = 0; i < window_len; ++i)
		{
			pcc_arr[j] += pcc_get_pcc(ctx,i);
			if( (i + 1) % groups == 0){
				j++;
			}
		}

		// for (int i = 0; i < groups; ++i)
		// {
		// 	printf("%b\n", );
		// }

		// Clear the pcc for next cycle
		pcc_free(ctx);

		// Extarct the index of the max, which represent our guessed bits
		guess = max(pcc_arr,groups);

		// Advance the refernece messaged of our guess
		ME_big_estimate(guess,M_arr,n,bits_guessed);

		// Reset all possible path to the guessed state for next cycle
		for(int i = 0; i < window_len; i++) {
			memcpy((void *) window[i], (const void*) M_arr, sizeof(msg_t) * n_sample);
		}

		// Upfate the private key, the mask is necessary as the guess has the LSB in the left
	    int mask = (1 << (bits_guessed - 1));
		for (int i = 0; i < bits_guessed; i++)
		{
			if(guess & mask){
				key_guessed[i + step] = 1;
			}else{
				key_guessed[i + step] = 0;
			}
		}

		step += bits_guessed;

		for (int i = 0; i < step; i++)
		{
			printf("%d", key_guessed[i]);
		}

		printf("\n");

		bigint_t key_tmp = private;
		for (int i = 0; i < step; i++)
		{
			printf("%d", key_tmp.numb[0] & 1);
			key_tmp = lsr(key_tmp,1);
		}

		printf("\n\n");



	}



	return 0;
}