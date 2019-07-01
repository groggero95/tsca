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
    n  = modulus
    c0 = precomputed value of: (2**nb) % n
    s0 = precomputed value of: m*(2**nb) % n
    nb = number of bit
*/
int me(unsigned int e, unsigned int n, unsigned int c0, unsigned int s0, unsigned int nb) {
    int c = c0;
    int s = s0;

    for (int i = 0, mask = 1; i < nb; i++,  mask =  mask << 1) {
        // Check if exponent bit is one
        if ( e & mask ) {
            // Multiply operation to incorporate the part of the message needed
            c = mm(c, s, n, nb);
        }
        // Square operation, square the message at each iteration
        s = mm(s, s, n, nb);
    }
    // Convert c into normal domain
    c = mm(c, 1, n, nb);
    return c;
};


bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb) {
    bigint_t c;
    bigint_t s;
    bigint_t one = init(ONE);

    // Initialize c with the value 1 in montgomry domain
    c = MM_big(k0, one, n, nb);
    // Initialize s with the message in montgomery domain
    s = MM_big(k0, m, n, nb);

    // Loop over all the bit in the exponent
    for (int i = 0; i < nb; i++) {
        // Test if LSB of the exponent is set
        if (e.numb[0] & 1) {
            // Multiply operation to incorporate the part of the message needed
            c = MM_big(c, s, n, nb);
        }
        // Square operation, square the message at each iteration
        s = MM_big(s, s, n, nb);
        // Shift right the exponet by one to consume the LSB each time
        e = lsr(e, 1);

    }
    // Convert c back into normal domain
    c = MM_big(c, one, n, nb);
    return c;
}


bigint_t ME_big_blind(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb) {
    bigint_t c;
    bigint_t s;

    // Initialize the blinding parameters as static to allow the squaring
    // of vi and vf to be propagated to the next call of this function
    static bigint_t vi = { .numb = VI_INIT };
    static bigint_t vf = { .numb = VF_INIT };
    bigint_t one = init(ONE);   


    // Initialize c with the value 1 in montgomry domain
    c = MM_big(k0, one, n, nb);
    // Initialize s with the message in montgomery domain
    s = MM_big(k0, m, n, nb);
    // Incorporate the blinding factor vi to make the input s
    // unknown to the attacker
    s = MM_big(s, vi, n, nb);

    // Loop over all the bit in the exponent
    for (int i = 0; i < nb; i++) {
        // Test if LSB of the exponent is set
        if (e.numb[0] & 1) {
            // Multiply operation to incorporate the part of the message needed
            c = MM_big(c, s, n, nb);
        }
        // Square operation, square the message at each iteration
        s = MM_big(s, s, n, nb);
        // Shift right the exponet by one to consume the LSB each time
        e = lsr(e, 1);

    }
    // Undo the effect of the blinding using the value vf
    c = MM_big(c, vf, n, nb);
    // Convert c back into normal domain
    c = MM_big(c, one, n, nb);

    // Square both vi and vf to avoid side channel attacks on those factor
    vi = MM_big(vi, vi, n, nb);
    vf = MM_big(vf, vf, n, nb);
    return c;
}


void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step) {
    
    // Generate a mask to consume the bit in bits from left to right
    int mask = (1 << (bits_step - 1));

    // Loop for the specified number of bit
    for (int i = 0; i < bits_step; i++)
    {
        // Perform the multiply and square step as the normal algorithm
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

