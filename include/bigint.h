#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
//Following number has to be a multiple of 32
#define INT_SIZE 1024
#define VAR_SIZE 32
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1

// Structure to be used for the operations
typedef struct bign{
    //Field to store the actual value
    uint32_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint32_t pos;
} bign_t;


int eq(bign_t first, bign_t second);
int df(bign_t first, bign_t second);
int gt(bign_t first, bign_t second);
int ge(bign_t first, bign_t second);
int lt(bign_t first, bign_t second);
int le(bign_t first, bign_t second);
int lsr(bign_t *a, int pl);
int lsl(bign_t *a, int pl);

#endif
