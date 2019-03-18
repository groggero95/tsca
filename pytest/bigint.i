%module bigint

%{
#define DISTANCE_SWIG_WITH_INIT
#include "bigint.h"
%}

extern int eq(bigint_t first, bigint_t second);
extern int df(bigint_t first, bigint_t second);
extern int gt(bigint_t first, bigint_t second);
extern int ge(bigint_t first, bigint_t second);
extern int lt(bigint_t first, bigint_t second);
extern int le(bigint_t first, bigint_t second);
extern int lsr(bigint_t *a, int pl);
extern int lsl(bigint_t *a, int pl);

extern bigint_t sum(bigint_t a, bigint_t b);
extern bigint_t sub(bigint_t a, bigint_t b);
typedef struct bigint{
    //Field to store the actual value
    var_t numb[NUMB_SIZE];
    //Field to store the alignment of the value
    uint16_t pos;
} bigint_t;
