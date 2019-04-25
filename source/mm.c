#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "mm.h"


int MM(int a, int b, int n, int nb) {

	int res = 0, mask;
	for ( int i = 0, mask = 1, ai, qi; i < nb; i++, mask =  mask << 1) {
		ai = (a & mask) >> i;
		qi = (res + (ai & b)) & k;
		res = (res + (tr[ai] & b) + (tr[qi] & n)) >> k;
	}

	return res;

}


bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb) {

	bigint_t a_masked;
	bigint_t res = init(ZERO);
	bigint_t mask = init(ONE);
	var_t ai, qi;

	for (int i = 0; i < nb; i++, mask = lsl(mask, 1) ) {
		a_masked = and (a, mask);
		ai = lsr(a_masked, i).numb[0];
		qi = (res.numb[0] + (ai & (b.numb[0]))) & 1;

		if (ai) {
			res = sum(res, b);
		}
		if (qi) {
			res = sum(res, n);
		}

		res = lsr(res, 1);

	}
	return res;
}


