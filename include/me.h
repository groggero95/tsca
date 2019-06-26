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

#ifndef ME_H
#define ME_H

#include "panda4x4.h"

int ME(int e, int m, int n, int c0, int s0, int nb);
bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb);
bigint_t ME_big_blind(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb);
void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step);

#endif