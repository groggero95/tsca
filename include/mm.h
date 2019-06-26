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

#include "bigint.h"

#ifndef MM_H
#define MM_H

#define k 1
static const int tr[] = {0, ~0};

int MM(int a, int b, int m, int nb);
bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb);
bigint_t MM_big_estimate(bigint_t a, bigint_t b, bigint_t n, int nb, uint32_t *cnt);

#endif
