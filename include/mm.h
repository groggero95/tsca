#ifndef MM_H
#define MM_H
#include "bigint.h"

#define k 1
const int tr[] = {0, ~0};

int MM(int a, int b, int m, int nb);
bigint_t MM_big(bigint_t *a, bigint_t *b, bigint_t *n, int nb);

#endif
