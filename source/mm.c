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
#include "mm.h"


int MM(int a, int b, int n, int nb) {

	int res = 0;
	for ( int i = 0, mask = 1, ai, qi; i < nb; i++, mask =  mask << 1) {
		ai = (a & mask) >> i;
		qi = (res + (ai & b)) & k;
		res = (res + (tr[ai] & b) + (tr[qi] & n)) >> k;
	}

	return res;

}


bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb) {

	bigint_t res = init(ZERO);
	var_t qi;

	for (int i = 0; i < nb+2; i++) {
		qi = (res.numb[0] + (a.numb[0] & b.numb[0])) & 1;

		if (a.numb[0] & 1 ) {
			res = sum(res, b);
		}
		if (qi) {
			res = sum(res, n);
		}

		res = lsr(res, 1);
		a = lsr(a,1);

	}
	return res;
}


bigint_t MM_big_estimate(bigint_t a, bigint_t b, bigint_t n, int nb, uint32_t *cnt) {

	bigint_t res = init(ZERO);
	var_t qi;

	for (int i = 0; i < nb+2; i++) {
		qi = (res.numb[0] + (a.numb[0] & b.numb[0])) & 1;

		if (a.numb[0] & 1) {
			res = sum(res, b);
			*cnt += 1;
		}
		if (qi) {
			res = sum(res, n);
			*cnt += 1;
		}

		res = lsr(res, 1);
		a = lsr(a,1);

	}
	return res;
}

