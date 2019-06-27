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
#include "bigint.h"
#include "me.h"
#include "mm.h"
#include "panda4x4.h"
#include "cipher.h"


/*  e  = exponent
    m  = message
    n  = modulus
    c0 = precomputed value of: (1<<nb) % n
    s0 = precomputed value of: m*(1<<nb) % n
    nb = number of bit
*/
int ME(int e, int m, int n, int c0, int s0, int nb) {
    int c = c0;
    int s = s0;

    for (int i = 0, mask = 1; i < nb; i++,  mask =  mask << 1) {
        if ( e & mask ) {
            c = MM(c, s, n, nb);
        }
        s = MM(s, s, n, nb);
    }
    c = MM(c, 1, n, nb);
    return c;
};


bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb) {
    bigint_t c;
    bigint_t s;
    bigint_t one = init(ONE);

    c = MM_big(k0, one, n, nb);
    s = MM_big(k0, m, n, nb);

    for (int i = 0; i < nb; i++) {
        if (e.numb[0] & 1) {
            c = MM_big(c, s, n, nb);
        }
        s = MM_big(s, s, n, nb);
        e = lsr(e, 1);

    }
    c = MM_big(c, one, n, nb);
    return c;
}


bigint_t ME_big_blind(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb) {
    bigint_t c;
    bigint_t s;
    static bigint_t vi = { .numb = VI_INIT };
    static bigint_t vf = { .numb = VF_INIT };
    bigint_t one = init(ONE);   



    c = MM_big(k0, one, n, nb);
    s = MM_big(k0, m, n, nb);
    s = MM_big(s, vi, n, nb);

    for (int i = 0; i < nb; i++) {
        if (e.numb[0] & 1) {
            c = MM_big(c, s, n, nb);
        }
        s = MM_big(s, s, n, nb);
        e = lsr(e, 1);

    }
    c = MM_big(c, vf, n, nb);
    c = MM_big(c, one, n, nb);
    vi = MM_big(vi, vi, n, nb);
    vf = MM_big(vf, vf, n, nb);
    return c;
}


void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step) {
    
    int mask = (1 << (bits_step - 1));
    for (int i = 0; i < bits_step; i++)
    {
        if (bits & mask) {
            #if ATTACK_MUL == 1
            m->c = MM_big_estimate(m->c, m->s, n, INT_SIZE, &(m->tot_est));
            #else
            m->c = MM_big(m->c, m->s, n, INT_SIZE);            
            #endif
        }
        #if ATTACK_SQUARE == 1
        m->s = MM_big_estimate(m->s, m->s, n, INT_SIZE, &(m->tot_est));
        #else
        m->s = MM_big(m->s, m->s, n, INT_SIZE);
        #endif          
        mask >>= 1;
    }

    return;
}

