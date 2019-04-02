#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "time_meas.h"
#include "me.h"
#include "mm.h"


int main(int argc, char **argv){

	int nb, mode;
	bigint_t a, b, n, res, k0;
	time_stats_t *timer = (time_stats_t*) malloc(sizeof(time_stats_t));

    a = init(argv[1]); // exponent
    b = init(argv[2]); // plain text
    n = init(argv[3]); // modulus
    k0 = init(argv[4]); // (1<<2nb) % n
	mode = atoi(argv[5]);	// ME (0) or MM (1)
    nb = INT_SIZE+2;

    reset_meas(timer);
	for (int i = 0; i < 10; ++i)
	{
		start_meas(timer);
		switch (mode){
	    	case 0:
				res = ME_big(a,b,n,k0,nb);
				break;
			case 1:
				res = MM_big(a,b,n,nb);
				break;
		}
	    stop_meas(timer);
	}

	printf("%lld\n", timer->min);
	free(timer);
	return 0;
}
