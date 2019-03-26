#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//Following number has to be a multiple of 32
#define INT_SIZE 1024
#define VAR_SIZE 32
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1
#define HEX_DIGIT VAR_SIZE/4

#if VAR_SIZE == 64
    #define UMAX 0xFFFFFFFFFFFFFFFF
    #define HIMASK 0xFFFFFFFF00000000
    #define LOMASK 0x00000000FFFFFFFF
    typedef uint64_t var_t;
#elif VAR_SIZE == 32
    #define UMAX 0xFFFFFFFF
    #define HIMASK 0xFFFF0000
    #define LOMASK 0x0000FFFF
    typedef uint32_t var_t;
#elif VAR_SIZE == 16
    #define UMAX 0xFFFF
    #define HIMASK 0xFF00
    #define LOMASK 0x00FF
    typedef uint16_t var_t;
#elif VAR_SIZE == 8
    #define UMAX 0xFF
    #define HIMASK 0xF0
    #define LOMASK 0x0F
    typedef unit8_t var_t;
#endif


// Structure to be used for the operations
typedef struct bigint{
    //Field to store the actual value
    var_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint16_t pos;
} bigint_t;


int eq(bigint_t *first, bigint_t *second);
int df(bigint_t *first, bigint_t *second);
int gt(bigint_t *first, bigint_t *second);
int ge(bigint_t *first, bigint_t *second);
int lt(bigint_t *first, bigint_t *second);
int le(bigint_t *first, bigint_t *second);

bigint_t and(bigint_t *a, bigint_t *b);
bigint_t or(bigint_t *a, bigint_t *b);
bigint_t not(bigint_t *a);
bigint_t xor(bigint_t *a, bigint_t *b);
int lsr(bigint_t *a, int pl);
int lsl(bigint_t *a, int pl);

bigint_t sum(bigint_t a, bigint_t b);
bigint_t sub(bigint_t a, bigint_t b);
bigint_t mul(bigint_t a, bigint_t b);

var_t sum_4_mul(var_t *a, var_t b, var_t *carry, int act);
void print_to_stdout(bigint_t *a);

#endif
