#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "mm.h"


int MM(int a, int b, int n, int nb){

	int res = 0;
	for( int i = 0, mask = 1, ai, qi; i < nb; i++, mask =  mask << 1){
		ai = (a & mask) >> i;
		qi = (res + (ai&b)) & k;
		res = (res + (tr[ai]&b) + (tr[qi]&n)) >> k;
	}

	return res;

}