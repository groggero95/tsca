#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "me.h"
#include "mm.h"

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
    bigint_t mask = init(ONE);
    bigint_t zero = init(ZERO);
    bigint_t one = init(ONE);
    bigint_t bit_and;


    c = MM_big(k0, one, n, nb);
    // Here is the error in this function somewhere
    s = MM_big(k0, m, n, nb);

    for (int i = 0; i < nb; i++) {
        bit_and = and (mask, e);
        if (df(zero, bit_and)) {
            c = MM_big(c, s, n, nb);
        }
        s = MM_big(s, s, n, nb);
        mask = lsl(mask, 1);
    }
    c = MM_big(c, one, n, nb);

    return c;
}


void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step) {
    
    int mask = (1 << (bits_step - 1));
    for (int i = 0; i < bits_step; i++)
    {
        if (bits & mask) {
            m->c = MM_big_estimate(m->c, m->s, n, INT_SIZE+2, &(m->tot_est));
        }
        m->s = MM_big_estimate(m->s, m->s, n, INT_SIZE+2, &(m->tot_est));  
        mask >>= 1;
    }

    printf("%d\n", m->tot_est);
    
    return;
}


