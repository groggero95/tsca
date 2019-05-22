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
		// // print_to_stdout(&n);
		// // printf("\n");
	}

}

int main(int argc, char* argv[]) {

	// Declaration of the key that we want ro attack and other parameters needed for the attack
	bigint_t n = init("0x674b89bb51449c6281854973613618a189e52553b7974cc31026f50ecd3df6af1a2b1c086d1cbc9a9527536b9ebbb81fba8e751de1e408391e42a1e4451beba1");
	bigint_t private = init("0x10a51386ea3dd4035647c8644a335ae399d6b5e5175934c63aba1eca948f367b53c00e59ca21937a03792a96005b57c8106ad20e2af98b69ca2fd5cda48a37fb");
	bigint_t k0 = init("0x4e45781ff3ebd436b6497fcc94150fe0a90e5cbeabc2a017830d7a33362fb1537a75823a04729682d0501a92c5b689236df22ce1b4a0cff93ba6708a4924ff8f");

	// define the structure which holds timing and messages
	uint64_t *T_arr;
	msg_t *M_arr;
	uint32_t n_sample = N_SAMPLES;

	T_arr = (uint64_t *) malloc(sizeof(uint64_t) * n_sample);
	M_arr = (msg_t *) malloc(sizeof(msg_t) * n_sample);

	read_plain(TIME_FILE, MSG_FILE, n_sample, T_arr, M_arr, n, k0);


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
		printf("Step: %d\n",step);

		for (int i = 0; i < step; i++)
		{
			printf("%d", key_guessed[i]);
		}

		printf("\n");

		for (int i = 0; i < step; i++)
		{
			printf("%d", lsr(private,i).numb[0] & 1);
		}

		printf("\n\n");


	}



	return 0;
}
