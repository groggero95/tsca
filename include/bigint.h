#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
//Following number has to be a multiple of 32
#define INT_SIZE 1024
#define VAR_SIZE 32
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1

// Structure to be used for the operations
#if VAR_SIZE == 64
typedef struct bint{
    #define UMAX 0xFFFFFFFFFFFFFFFF
    //Field to store the actual value
    uint64_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint64_t pos;
} bint_t;
#elif VAR_SIZE == 32
typedef struct bint{
    #define UMAX 0xFFFFFFFF
    //Field to store the actual value
    uint32_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint32_t pos;
} bint_t;
#elif VAR_SIZE = 16
typedef struct bint{
    #define UMAX = 0xFFFF
    //Field to store the actual value
    uint16_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint16_t pos;
} bint_t;
#elif VAR_SIZE == 8
typedef struct bint{
    #define UMAX = 0xFF
    //Field to store the actual value
    uint8_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint8_t pos;
} bint_t;
#endif


int eq(bigint_t first, bigint_t second);
int df(bigint_t first, bigint_t second);
int gt(bigint_t first, bigint_t second);
int ge(bigint_t first, bigint_t second);
int lt(bigint_t first, bigint_t second);
int le(bigint_t first, bigint_t second);
int lsr(bigint_t *a, int pl);
int lsl(bigint_t *a, int pl);

bint_t sum(bint_t a, bint_t b);
bint_t sub(bint_t a, bint_t b);

#endif
