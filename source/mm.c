#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "mm.h"


int MM(int a, int b, int n, int nb){

	int res = 0, mask;
	for( int i = 0, mask = 1, ai, qi; i < nb; i++, mask =  mask << 1){
		ai = (a & mask) >> i;
		qi = (res + (ai&b)) & k;
		res = (res + (tr[ai]&b) + (tr[qi]&n)) >> k;
	}

	return res;

}


bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb){
	
	bigint_t res.init;
	bigint_t mask;
	for (int i = 0, mask.init; i < nb; ++i, lsl(mask,1) ){
		ai = 
		qi = 
	}
}

