#ifndef ME_H
#define ME_H

#include "panda4x4.h"

int ME(int e, int m, int n, int c0, int s0, int nb);
bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb);
void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step);

#endif