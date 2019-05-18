#include "bigint.h"

void print_to_stdout(bigint_t *a) {
    int i;
    xil_printf("0x");
    for (i = NUMB_SIZE - 1; i >= 0; i--) {
        xil_printf("%08x", a->numb[i]);
    }
//    xil_printf("\r\n");
    return;
}

// Return 0 if different, else 1; -1 if not aligned
int eq(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //     return -1;
    // }
    for (int i = 0; i < NUMB_SIZE; i++) {
        if (first.numb[i] != second.numb[i])
            return 0;
    }
    return 1;
}

// Return 0 if equal, else 1; -1 if not aligned
int df(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //     return -1;
    // }
    for (int i = 0; i < NUMB_SIZE; i++) {
        if (first.numb[i] != second.numb[i])
            return 1;
    }
    return 0;
}

// Return 1 if strictly gt, else 0; -1 if not aligned
int gt(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //     return -1;
    // }
    for (int i = NUMB_SIZE - 1; i >= 0; i--) {
        if (first.numb[i] < second.numb[i])
            return 0;
        else if (first.numb[i] > second.numb[i])
            return 1;
    }
    return 0;
}

// Return 1 if ge, else 0; -1 if not aligned
int ge(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //     return -1;
    // }
    for (int i = NUMB_SIZE - 1; i >= 0; i--) {
        if (first.numb[i] < second.numb[i])
            return 0;
        else if (first.numb[i] > second.numb[i])
            return 1;
    }
    return 1;
}


// Return 1 if strictly lt, else 0; -1 if not aligned
int lt(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //     return -1;
    // }
    for (int i = NUMB_SIZE - 1; i >= 0; i--) {
        if (first.numb[i] > second.numb[i])
            return 0;
        else if (first.numb[i] < second.numb[i])
            return 1;
    }
    return 0;
}

// Return 1 if le, else 0; -1 if not aligned
int le(bigint_t first, bigint_t second) {
    // if (first->pos != second->pos){
    //         return -1;
    // }
    for (int i = NUMB_SIZE; i >= 0; i--) {
        if (first.numb[i] > second.numb[i])
            return 0;
        else if (first.numb[i] < second.numb[i])
            return 1;
    }
    return 1;
}


// Bitwise and &
bigint_t and (bigint_t a, bigint_t b) {

    bigint_t data_res;
    for (int i = 0; i < NUMB_SIZE; i++) {
        data_res.numb[i] = a.numb[i] & b.numb[i];
    }
    // data_res.pos = 0;
    // data_res.numb[NUMB_SIZE-1] = 0;
    return data_res;
}


// Bitwise or |
bigint_t or (bigint_t a, bigint_t b) {

    bigint_t data_res;
    for (int i = 0; i < NUMB_SIZE; i++) {
        data_res.numb[i] = a.numb[i] | b.numb[i];
    }
    // data_res.pos = 0;
    // data_res.numb[NUMB_SIZE-1] = 0;
    return data_res;
}

// Bitwise not ~
bigint_t not(bigint_t a) {

    bigint_t data_res;
    for (int i = 0; i < NUMB_SIZE; i++) {
        data_res.numb[i] = ~(a.numb[i]);
    }
    // data_res.pos = 0;
    // data_res.numb[NUMB_SIZE-1] = 0;
    return data_res;
}

// Bitwise xor ^
bigint_t xor(bigint_t a, bigint_t b) {

    bigint_t data_res;
    for (int i = 0; i < NUMB_SIZE; i++) {
        data_res.numb[i] = a.numb[i] ^ b.numb[i];
    }
    // data_res.pos = 0;
    // data_res.numb[NUMB_SIZE-1] = 0;
    return data_res;
}


// Logically shift right a, by pl places
bigint_t lsr(bigint_t a, int pl) {

    bigint_t data_res;
    int full_shift = pl / VAR_SIZE;
    pl = pl % VAR_SIZE;
    for (int k = 0; k < NUMB_SIZE; k++) {
        if ((k < NUMB_SIZE - full_shift - 1)) {
            data_res.numb[k] = ((pl ? (a.numb[k + full_shift + 1] << (VAR_SIZE - pl)) : 0) | (a.numb[k + full_shift] >> pl));
        } else if (k ==  NUMB_SIZE - full_shift - 1) {
            data_res.numb[k] = (a.numb[k + full_shift] >> pl);
        } else {
            data_res.numb[k] = 0;
        }
    }
    if (full_shift == 0)
        data_res.numb[NUMB_SIZE - 1] = a.numb[NUMB_SIZE - 1] >> pl;
    return data_res;
}

// Logically shift left a, by pl places
bigint_t lsl(bigint_t a, int pl) {

    bigint_t data_res;
    int full_shift = pl / VAR_SIZE;
    pl = pl % VAR_SIZE;
    for (int k = NUMB_SIZE - 1; k >= 0; k--) {
        if (k > full_shift) {
            data_res.numb[k] = ((a.numb[k - full_shift] << pl) | (pl ? (a.numb[k - full_shift - 1] >> (VAR_SIZE - pl)) : 0));
        } else if (k == full_shift) {
            data_res.numb[k] = (a.numb[k - full_shift] << pl);
        } else {
            data_res.numb[k] = 0;
        }
    }
    if (full_shift == 0)
        data_res.numb[0] = a.numb[0] << pl;
    return data_res;
}


// Sum, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sum(bigint_t a, bigint_t b) {

    bigint_t data_res;

    var_t carry = 0, new_carry = 0;
    int i;

    for (i = 0; i < NUMB_SIZE ; i++) {
        data_res.numb[i] = a.numb[i] + b.numb[i];
        new_carry = a.numb[i] > data_res.numb[i];
        data_res.numb[i] = data_res.numb[i] + carry;
        carry = new_carry | (a.numb[i] > data_res.numb[i]);
        //printf("stage %d sum = %x\n", i, data_res.numb[i]);
    }

    //data_res.pos = INT_SIZE + carry*VAR_SIZE;
    //data_res.numb[i] = carry; //+ a.numb[i] + b.numb[i];
    //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);

    return data_res;
}

// TODO fix the sub, problems are here
// SUB, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sub(bigint_t a, bigint_t b) {

    bigint_t data_res;

    var_t borrow = 0, new_borrow = 0;
    int i;

    for (i = 0; i < NUMB_SIZE; i++) {
        data_res.numb[i] = a.numb[i] - b.numb[i];
        new_borrow = data_res.numb[i] > a.numb[i];
        data_res.numb[i] = data_res.numb[i] - borrow;
        borrow = new_borrow | (data_res.numb[i] > a.numb[i]);
        //printf("stage %d sub = %x\n", i, data_res.numb[i]);
    }

    // data_res.pos = INT_SIZE + borrow*VAR_SIZE;
    // data_res.numb[i] = 0-borrow;
    //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);

    return data_res;
}

var_t sum_4_mul(var_t *a, var_t b, var_t *carry, int act) {

    var_t data_res;
    var_t new_carry = 0;

    data_res = (*a) + b;
    new_carry = (*a) > data_res;
    if (act == 1) {
        data_res = data_res + (*carry);
        new_carry = new_carry | ((*a) > data_res);
    } else {
        (*carry) = 0;
    }

    (*a) = data_res;

    return new_carry;
}

// Sum, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t mul(bigint_t a, bigint_t b) {

    int i, j;
    var_t par_res[4];           // stores the 4 partial mult of a NxN multiplication
    var_t sum[2 * (NUMB_SIZE)]; // stores the multiplication result, which at max needs double the max size of the operands
    var_t carry[2 * (NUMB_SIZE) + 1]; // stores the carry

    bigint_t data_res;

    // initialize sum and carry vector
    for (i = 0; i < 2 * (NUMB_SIZE) ; i++) {
        sum[i] = 0;
        carry[i] = 0;
    }
    carry[i] = 0;

    // computes the multiplication
    for (i = 0; i < NUMB_SIZE ; i++) {
        for (j = 0; j < NUMB_SIZE; j++) {
            // perform 4 mult on half of the max VAR_SIZE bits, to avoid overflow
            par_res[0] = (b.numb[i] & LOMASK) * (a.numb[j] & LOMASK);
            par_res[1] = (b.numb[i] & LOMASK) * ((a.numb[j] & HIMASK) >> (VAR_SIZE / 2));
            par_res[2] = ((b.numb[i] & HIMASK) >> (VAR_SIZE / 2)) * (a.numb[j] & LOMASK);
            par_res[3] = ((b.numb[i] & HIMASK) >> (VAR_SIZE / 2)) * ((a.numb[j] & HIMASK) >> (VAR_SIZE / 2));

            // compine the 4 factors just computed, check for sum overflows
            //sum[j] = par_res[0] + par_res[1] << 16 + par_res[2] << 16 + par_res[3] << 32;
            //sum[i+j] = sum[i+j] + par_res[0] + (par_res[1] << (VAR_SIZE/2)) + (par_res[2] << (VAR_SIZE/2));

            carry[i + j + 1] += sum_4_mul(&sum[i + j], par_res[0], &carry[i + j], 1);
            carry[i + j + 1] += sum_4_mul(&sum[i + j], (par_res[1] << (VAR_SIZE / 2)), &carry[i + j], 0);
            carry[i + j + 1] += sum_4_mul(&sum[i + j], (par_res[2] << (VAR_SIZE / 2)), &carry[i + j], 0);

            //sum[j+1] = par_res[0] >> 32 + par_res[1] >> 16 + par_res[2] >> 16 + par_res[3];
            if ((i + j) != 2 * (NUMB_SIZE - 1 - 1)) {
                //sum[i+j+1] = sum[i+j+1] + (par_res[1] >> (VAR_SIZE/2)) + (par_res[2] >> (VAR_SIZE/2)) + par_res[3];
                carry[i + j + 2] += sum_4_mul(&sum[i + j + 1], (par_res[1] >> (VAR_SIZE / 2)), &carry[i + j + 1], 1);
                carry[i + j + 2] += sum_4_mul(&sum[i + j + 1], (par_res[2] >> (VAR_SIZE / 2)), &carry[i + j + 1], 0);
                carry[i + j + 2] += sum_4_mul(&sum[i + j + 1], par_res[3], &carry[i + j + 1], 0);
            }
        }
    }

    for (i = 0; i < NUMB_SIZE ; i++) {
        data_res.numb[i] = sum[i];
        //printf("stage %d mul = %x\n", i, data_res.numb[i]);
    }
    //data_res.numb[i] = sum[i];

    //data_res.pos = INT_SIZE + VAR_SIZE*!(data_res.numb[NUMB_SIZE-1] == 0);
    //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[NUMB_SIZE-1]);


    return data_res;
}

bigint_t init(const char *s) {

    char temp[HEX_DIGIT + 1];
    temp[HEX_DIGIT] = '\0';
    int max_i = NUMB_SIZE;
    bigint_t data_res;
    for (int i = max_i - 2; i >= 0; i--) {
        strncpy(temp, s + 2 + HEX_DIGIT * (max_i - 2 - i), HEX_DIGIT);
//        xil_printf("%s\n\r",temp);
        data_res.numb[i] = (var_t) strtoul(temp, NULL, 16);
    }
    data_res.numb[max_i - 1] = 0;
    //data_res.pos=0;
    return data_res;
}

bigint_t rand_b( void ) {
    int i;
    bigint_t data_res;
    for (i = 0; i < NUMB_SIZE-1; i++) {
        data_res.numb[i] = (var_t) rand();
    }
    data_res.numb[NUMB_SIZE-1] = 0;
    return data_res;
}


bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb) {

    bigint_t res = init(ZERO);
    var_t qi;

    for (int i = 0; i < nb; i++) {
        qi = (res.numb[0] + (a.numb[0] & b.numb[0])) & 1;

        if (a.numb[0] & 1 ) {
            res = sum(res, b);
        }
        if (qi) {
            res = sum(res, n);
        }

        res = lsr(res, 1);
        a = lsr(a,1);

    }
    return res;
}

bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb) {
    bigint_t c;
    bigint_t s;
    bigint_t mask = init(ONE);
    bigint_t zero = init(ZERO);
    bigint_t one = init(ONE);
    bigint_t bit_and;

    // xil_printf("\r\n");
    // print_to_stdout(&mask);
    // xil_printf("\r\n");
    // print_to_stdout(&zero);
    // xil_printf("\r\n");
    // print_to_stdout(&one);
    // xil_printf("\r\n"); 

    c = MM_big(k0, one, n, nb);
    s = MM_big(k0, m, n, nb);

    // xil_printf("\r\n");
    // print_to_stdout(&c);
    // xil_printf("\r\n");
    // print_to_stdout(&s);
    // xil_printf("\r\n");

    for (int i = 0; i < nb; i++) {
        bit_and = and (mask, e);
        if (df(zero, bit_and)) {
            c = MM_big(c, s, n, nb);
        }
        s = MM_big(s, s, n, nb);
        mask = lsl(mask, 1);

    }
    c = MM_big(c, one, n, nb);
    return c;
}