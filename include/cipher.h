#ifndef CIPHER_H
#define CIPHER_H

#include "bigint.h"

#define BITS INT_SIZE
#define VERSION 0
#define TESTNUM 10000
#define BLINDING 0

// Structure to be used for the operations
typedef struct key_pair{
  bigint_t public;
  bigint_t modulus;
  bigint_t private;
  bigint_t k0;
} key_p;

key_p pair;

void init_pair(void);

#endif
