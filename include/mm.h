#include "bigint.h"

#ifndef MM_H
#define MM_H

#define k 1
static const int tr[] = {0, ~0};

int MM(int a, int b, int m, int nb);
bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb);
bigint_t MM_big_estimate(bigint_t a, bigint_t b, bigint_t n, int nb, uint32_t *cnt);

#endif
