#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "mm.h"


int MM(int a, int b, int m, int nb){

	//printf("a: %d, b: %d, m: %d, nb: %d\n",a,b,m,nb );
	int b0 = b & 0x01;
	int res = 0;
	for( int i = 0, mask = 1, ai, qi, res0; i < nb; i++, mask =  mask << 1){
		ai = (a & mask) >> i;
		res0 = res & 0x01;
		qi = (res0 + ai*b0) % r;
		res = (res + ai*b + qi*m) >> 1;
		//printf("q%d: %d, a%d: %d, s: %d\n",i, qi, i, ai,res);

	}

	//printf("%d\n", res);
	return res;

}