/*
 * Copyright (C) EURECOM, Telecom Paris
 *
 * Prof. Renaud PACALET         <renaud.pacalet@telecom-paristech.fr>
 * Alberto ANSELMO              <Alberto.Anselmo@eurecom.fr>
 * Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
 * Fausto CHIATANTE             <Fausto.Chiatante@eurecom.fr>
 * Giulio ROGGERO               <Giulio.Roggero@eurecom.fr>
 *
 * This file must be used under the terms of the CeCILL. This source
 * file is licensed as described in the file COPYING, which you should
 * have received as part of this distribution. The terms are also
 * available at:
 * http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "time_meas.h"
#include "me.h"
#include "mm.h"
#include "cipher.h"

#define REPETITIONS 10

const key_p pair = { .public 	= { .numb = PUBLIC_INIT},
					 .modulus 	= { .numb = MODULUS_INIT},
					 .private 	= { .numb = PRIVATE_INIT},
					 .k0 		= { .numb = K0_INIT},
					 .vi 		= { .numb = VI_INIT},
					 .vf 		= { .numb = VF_INIT}
					};

int main(int argc, char **argv){

	int nb, n_rnd, n_rep, n_test, mode;
	bigint_t a, b, n, res, k0;
	time_stats_t *timer = (time_stats_t*) malloc(sizeof(time_stats_t));
	FILE *f_time, *f_plain;

	// init_pair(); // init the key according to the settings in cipher.h

	f_time = fopen("data/TIME.BIN","w");
	f_plain = fopen("data/PLAIN.BIN","w");

	if (argc == 1){
		n_test = 1000;
		mode = 0;
	} else if (argc == 2){
		n_test = atoi(argv[1]);
		mode = 0;
	} else if (argc == 3){
		n_test = atoi(argv[1]);
		if (strcmp(argv[2],"-e")==0){
			mode = 0;
		} else if (strcmp(argv[2],"-m")==0){
			mode = 1;
		} else {
			printf("Usage: ./timing [N_TEST] [-e/-m]\n");
			return -1;
		}
	} else {
		printf("Usage: ./timing [N_TEST] [-m]\n");
		return -1;
	}

	nb = INT_SIZE;
	reset_meas(timer);

	printf("Starting sample acquisitions\n");
	if (mode == 0){
		for (int j = 0; j < n_test; ++j)
		{
			b = rand_b();
			for (int i = 0; i < REPETITIONS; ++i)
			{
				start_meas(timer);
				#if BLINDING == 0
				res = ME_big(pair.private, b, pair.modulus, pair.k0, nb);
				#else
				res = ME_big_blind(pair.private, b, pair.modulus, pair.k0, nb);
				#endif
			  	stop_meas(timer);
			}
			fwrite(&(timer->min),1,sizeof(timer->min),f_time);
			fwrite(b.numb,1,INT_SIZE/8,f_plain);
			printf("\riteration %7d: %lld",j+1,(timer->min));
			fflush(stdout);
			reset_meas(timer);
		}
	} else if(mode == 1){
  	for(int j = 0; j < n_test; j++)
	  	{
				b = rand_b();
		    	for (int i = 0; i < REPETITIONS; ++i)
				{
					start_meas(timer);
					res = MM_big(pair.private,b,pair.modulus,nb);
				  stop_meas(timer);
				}
			fwrite( &(timer->min) ,1,sizeof(timer->min),f_time);
			fwrite(b.numb,1,INT_SIZE/8,f_plain);
			printf("\riteration %7d: %lld",j+1,(timer->min));
			fflush(stdout);
			reset_meas(timer);
		}
	}

	fflush(f_time);
	fflush(f_plain);
	fclose(f_time);
	fclose(f_plain);

	free(timer);
	printf("\nAll acquisitions obtained.\n");
	return 0;
}
