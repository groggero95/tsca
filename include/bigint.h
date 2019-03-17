#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
//Following number has to be a multiple of 32
#define INT_SIZE 1024
#define VAR_SIZE 32
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1

// Structure to be used for the operations
typedef struct bigint{
    //Field to store the actual value
    uint32_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint32_t pos;
} bigint_t;


int eq(bigint_t first, bigint_t second);
int df(bigint_t first, bigint_t second);
int gt(bigint_t first, bigint_t second);
int ge(bigint_t first, bigint_t second);
int lt(bigint_t first, bigint_t second);
int le(bigint_t first, bigint_t second);
int lsr(bigint_t *a, int pl);
int lsl(bigint_t *a, int pl);

#endif
