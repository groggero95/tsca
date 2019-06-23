#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "time_meas.h"
#include "me.h"
#include "mm.h"
#include "cipher.h"

#define REPETITIONS 10
#define MODE 0
#define TESTNUM 10000

const key_p pair = { .public 	= { .numb = PUBLIC_INIT},
					 .modulus 	= { .numb = MODULUS_INIT},
					 .private 	= { .numb = PRIVATE_INIT},
					 .k0 		= { .numb = K0_INIT},
					 .vi 		= { .numb = VI_INIT},
					 .vf 		= { .numb = VF_INIT}
					};

int main(int argc, char **argv){

	int nb, n_rnd, n_rep;
	bigint_t a, b, n, res, k0;
	time_stats_t *timer = (time_stats_t*) malloc(sizeof(time_stats_t));
	FILE *f_time, *f_plain;

	// init_pair(); // init the key according to the settings in cipher.h

	f_time = fopen("data/TIME.BIN","w");
	f_plain = fopen("data/PLAIN.BIN","w");

	nb = INT_SIZE;
	reset_meas(timer);

	printf("Starting sample acquisitions\n");
	if (MODE == 0){
		for (int j = 0; j < TESTNUM; ++j)
		{
			b = rand_b();
			for (int i = 0; i < REPETITIONS; ++i)
			{
				start_meas(timer);
				res = ME_big(pair.private, b, pair.modulus, pair.k0, nb);
			  	stop_meas(timer);
			}
			fwrite(&(timer->min),1,sizeof(timer->min),f_time);
			fwrite(b.numb,1,INT_SIZE/8,f_plain);
			printf("\riteration %7d: %lld",j+1,(timer->min));
			fflush(stdout);
			reset_meas(timer);
		}
	} else if(MODE == 1){
  	for(int j = 0; j < TESTNUM; j++)
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
