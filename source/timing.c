#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "time_meas.h"
#include "me.h"
#include "mm.h"


int main(int argc, char **argv){

	int nb, mode, n_rnd, n_rep;
	bigint_t a, b, n, res, k0;
	time_stats_t *timer = (time_stats_t*) malloc(sizeof(time_stats_t));
	FILE *f_time, *f_plain;

	if (argc > 1){
	    a = init(argv[1]); // exponent
	    b = init(argv[2]); // plain text
	    n = init(argv[3]); // modulus
	    k0 = init(argv[4]); // (1<<2nb) % n
		mode = atoi(argv[5]);	// ME (0) or MM (1)
	}else{
		a = init("0x0745812bb1ffacf0b5d6200be2ced7d5"); // exponent
	    n = init("0xc26e8d2105e3454baf122700611e915d"); // modulus
	    k0 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f"); // (1<<2nb) % n
		mode = 0;	// ME (0) or MM (1)
		n_rnd = 1;
		n_rep = 10;
		f_time = fopen("data/TIME.BIN","w");
		f_plain = fopen("data/PLAIN.BIN","w");
	}
    nb = INT_SIZE+2;


	reset_meas(timer);


	if (argc > 1){
	    if (mode == 0){
			for (int i = 0; i < 10; ++i)
			{
				start_meas(timer);
				res = ME_big(a,b,n,k0,nb);
			    stop_meas(timer);
			}
	    }else if(mode == 1){
	    	for (int i = 0; i < 10; ++i)
			{
				start_meas(timer);
				res = MM_big(a,b,n,nb);
			    stop_meas(timer);
			}
		}

		printf("%lld\n", timer->min);
	}else{
		printf("Starting sample acquisitions\n");
		if (mode == 0){
			for (int j = 0; j < n_rnd; ++j)
			{
				b = rand_b();
				for (int i = 0; i < n_rep; ++i)
				{
					start_meas(timer);
					res = ME_big(a,b,n,k0,nb);
				    stop_meas(timer);
				}
				fwrite(&(timer->min),1,sizeof(timer->min),f_time);
				fwrite(b.numb,1,INT_SIZE/8,f_plain);
			}
			
	    }else if(mode == 1){
	    	for(int j = 0; j < n_rnd; j++)
	    	{
				b = rand_b();
		    	for (int i = 0; i < n_rep; ++i)
				{
					start_meas(timer);
					res = MM_big(a,b,n,nb);
				    stop_meas(timer);
				}
			fwrite( &(timer->min) ,1,sizeof(timer->min),f_time);
			fwrite(b.numb,1,INT_SIZE/8,f_plain);
			}		
		}

		fflush(f_time);
		fflush(f_plain);
		fclose(f_time);
		fclose(f_plain);
	}

	free(timer);
	return 0;
}
