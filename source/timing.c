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
		a = init("0x60c0fdfc71f6698b0a2529c4d64712fb135a399d27fd9f1b720aa52268c90a91"); // exponent
	    n = init("0x8bd09d3203b60a2255885d348eb020af8d2c040a399c6e07e40ee478ac4a2881"); // modulus
	    k0 = init("0x2801ebbfecc8bf74941d23e9fb4edc7fb51a964a86ae40027ad15a791325ffc5"); // (1<<2nb) % n
		mode = 0;	// ME (0) or MM (1)
		n_rnd = 1000;
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
				reset_meas(timer);

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
