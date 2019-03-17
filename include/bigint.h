#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
//Following number has to be a multiple of 32
#define INT_SIZE 1024
#define VAR_SIZE 32
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1

#if VAR_SIZE == 64
    #define UMAX 0xFFFFFFFFFFFFFFFF
    typedef uint64_t var_t;
#elif VAR_SIZE == 32
    #define UMAX 0xFFFFFFFF
    typedef uint32_t var_t;
#elif VAR_SIZE == 16
    #define UMAX 0xFFFF
    typedef uint16_t var_t;
#elif VAR_SIZE == 8
    #define UMAX 0xFF
    typedef unit8_t var_t;
#endif


// Structure to be used for the operations
typedef struct bigint{
    //Field to store the actual value
    var_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint16_t pos;
} bigint_t;


int eq(bigint_t first, bigint_t second);
int df(bigint_t first, bigint_t second);
int gt(bigint_t first, bigint_t second);
int ge(bigint_t first, bigint_t second);
int lt(bigint_t first, bigint_t second);
int le(bigint_t first, bigint_t second);
int lsr(bigint_t *a, int pl);
int lsl(bigint_t *a, int pl);

bigint_t sum(bigint_t a, bigint_t b);
bigint_t sub(bigint_t a, bigint_t b);

#endif
