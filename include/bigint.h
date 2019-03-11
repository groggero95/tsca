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



#endif
