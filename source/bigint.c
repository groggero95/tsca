#include "../include/bigint.h"
// Return 0 if different, else 1; -1 if not aligned
int eq(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
        return -1;
    }
    for(int i=0; i<NUMB_SIZE-1; i++){
        if(first->numb[i]!=second->numb[i])
           return 0;
    }
    return 1;
}

// Return 0 if equal, else 1; -1 if not aligned
int df(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
        return -1;
    }
    for(int i=0; i<NUMB_SIZE-1; i++){
        if(first->numb[i]!=second->numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if strictly gt, else 0; -1 if not aligned
int gt(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first->numb[i] < second->numb[i])
           return 0;
        else if(first->numb[i] > second->numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if ge, else 0; -1 if not aligned
int ge(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first->numb[i]<second->numb[i])
           return 0;
        else if(first->numb[i] > second->numb[i])
           return 1;
    }
    return 1;
}


// Return 1 if strictly lt, else 0; -1 if not aligned
int lt(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first->numb[i] > second->numb[i])
           return 0;
        else if(first->numb[i] < second->numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if le, else 0; -1 if not aligned
int le(bigint_t *first, bigint_t *second){
    if (first->pos != second->pos){
            return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first->numb[i] > second->numb[i])
            return 0;
        else if(first->numb[i] < second->numb[i])
            return 1;
        }
    return 1;
}


// Bitwise and &
bigint_t and(bigint_t *a, bigint_t *b){

    bigint_t data_res;
    for(int i=0; i<NUMB_SIZE-1; i++){
        data_res.numb[i] = a->numb[i] & b->numb[i];
    }
    return data_res;
}


// Bitwise or |
bigint_t or(bigint_t *a, bigint_t *b){

    bigint_t data_res;
    for(int i=0; i<NUMB_SIZE-1; i++){
        data_res.numb[i] = a->numb[i] | b->numb[i];
    }
    return data_res;
}

// Bitwise not ~
bigint_t not(bigint_t *a){

    bigint_t data_res;
    for(int i=0; i<NUMB_SIZE-1; i++){
      data_res.numb[i] ~= a->numb[i];
    }
    return data_res;
}

// Bitwise xor ^
bigint_t xor(bigint_t *a, bigint_t *b){

    bigint_t data_res;
    for(int i=0; i<NUMB_SIZE-1; i++){
        data_res.numb[i] = a->numb[i] ^ b->numb[i];
    }
    return data_res;
}

// Logically shift right a, by pl places
int lsr(bigint_t *a, int pl){

  if (pl < INT_SIZE+VAR_SIZE) {
    var_t tmp;
    int full_shift=pl/VAR_SIZE;
    pl = pl % VAR_SIZE;
    if (full_shift){
        //Full shift of VAR_SIZE*full_shift bits done
        for(int k=0; k<NUMB_SIZE; k++){
            if (k < NUMB_SIZE - full_shift){
                a->numb[k-full_shift]=a->numb[k];
            } else {
                a->numb[k]=0;
            }
        }
    }

    for (int i=0; i<NUMB_SIZE-1; i++){
        a->numb[i] >>= pl;
        tmp = a->numb[i+1];
        if (pl != 0)
            tmp <<= (VAR_SIZE-pl);
        else
            tmp = 0;
        a->numb[i] = a->numb[i] | tmp;
    }
    a->numb[NUMB_SIZE-1] >>= pl;

  } else {
      for(int k=0; k<NUMB_SIZE; k++){
          a->numb[k]=0;
      }
  }

    return 1;
}

// Logically shift left a, by pl places
int lsl(bigint_t *a, int pl){
  if (pl < INT_SIZE+VAR_SIZE) {
    var_t tmp;
    int full_shift=pl/VAR_SIZE;
    pl = pl % VAR_SIZE;
    if (full_shift){
        //Full shift of VAR_SIZE*full_shift bits done
        for(int k=NUMB_SIZE-1; k>=0; k--){
            if (k >= full_shift){
              a->numb[k]=a->numb[k-full_shift];
            } else {
              a->numb[k]=0;
            }
        }
    }

    for (int i=NUMB_SIZE-1; i>0; i--){
        a->numb[i] <<= pl;
        tmp = a->numb[i-1];
        if (pl != 0)
            tmp >>= (VAR_SIZE-pl);
        else
            tmp = 0;
        a->numb[i] = a->numb[i] | tmp;
    }
    a->numb[0] <<= pl;

  } else {
    for(int k=NUMB_SIZE-1; k>=0; k--){
        a->numb[k]=0;
      }
    }
    return 1;
}

void print_to_stdout(bigint_t *a){
  int i;
  printf("0x");
  for(i=NUMB_SIZE-1;i>=0; i--){
    printf("%08x",(*a).numb[i]);
  }
//  printf("\n");
  return ;
}

// Sum, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sum(bigint_t *a, bigint_t *b){

  bigint_t data_res;

  var_t carry = 0, new_carry = 0;
  int i;

  for (i = 0; i < NUMB_SIZE-1 ; i++){
    data_res.numb[i] = a->numb[i] + b->numb[i];
    new_carry = a->numb[i] > data_res.numb[i];
    data_res.numb[i] = data_res.numb[i] + carry;
    carry = new_carry | (a->numb[i] > data_res.numb[i]);
    //printf("stage %d sum = %x\n", i, data_res.numb[i]);
  }

  data_res.pos = INT_SIZE + carry*VAR_SIZE;
  data_res.numb[i] = carry;
  //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);

  return data_res;
}

// SUB, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sub(bigint_t *a, bigint_t *b){

  bigint_t data_res;

  var_t borrow = 0, new_borrow=0;
  int i;

  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    data_res.numb[i] = a->numb[i] - b->numb[i];
    new_borrow = data_res.numb[i] > a->numb[i];
    data_res.numb[i] = data_res.numb[i] - borrow;
    borrow = new_borrow | (data_res.numb[i] > a->numb[i]);
    //printf("stage %d sub = %x\n", i, data_res.numb[i]);
  }

  data_res.pos = INT_SIZE + borrow*VAR_SIZE;
  data_res.numb[i] = 0-borrow;
  //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);

  return data_res;
}

var_t sum_4_mul(var_t *a, var_t b, var_t *carry, int act){

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
bigint_t mul(bigint_t *a, bigint_t *b){

  int i, j;
  var_t par_res[4];           // stores the 4 partial mult of a NxN multiplication
  var_t sum[2*(NUMB_SIZE-1)]; // stores the multiplication result, which at max needs double the max size of the operands
  var_t carry[2*(NUMB_SIZE-1)+1]; // stores the carry

  bigint_t data_res;

  // initialize sum and carry vector
  for (i = 0; i < 2*(NUMB_SIZE-1) ; i++) {
    sum[i] = 0;
    carry[i] = 0;
  }
  carry[i] = 0;

  // computes the multiplication
  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    for (j = 0; j < NUMB_SIZE-1; j++) {
      // perform 4 mult on half of the max VAR_SIZE bits, to avoid overflow
      par_res[0] = (b->numb[i]&LOMASK) * (a->numb[j]&LOMASK);
      par_res[1] = (b->numb[i]&LOMASK) * ((a->numb[j]&HIMASK) >> (VAR_SIZE/2));
      par_res[2] = ((b->numb[i]&HIMASK) >> (VAR_SIZE/2)) * (a->numb[j]&LOMASK);
      par_res[3] = ((b->numb[i]&HIMASK) >> (VAR_SIZE/2)) * ((a->numb[j]&HIMASK) >> (VAR_SIZE/2));

      // compine the 4 factors just computed, check for sum overflows
      //sum[j] = par_res[0] + par_res[1] << 16 + par_res[2] << 16 + par_res[3] << 32;
      //sum[i+j] = sum[i+j] + par_res[0] + (par_res[1] << (VAR_SIZE/2)) + (par_res[2] << (VAR_SIZE/2));

      carry[i+j+1] += sum_4_mul(&sum[i+j],par_res[0],&carry[i+j],1);
      carry[i+j+1] += sum_4_mul(&sum[i+j],(par_res[1] << (VAR_SIZE/2)),&carry[i+j],0);
      carry[i+j+1] += sum_4_mul(&sum[i+j],(par_res[2] << (VAR_SIZE/2)),&carry[i+j],0);

      //sum[j+1] = par_res[0] >> 32 + par_res[1] >> 16 + par_res[2] >> 16 + par_res[3];
      if ((i+j) != 2*(NUMB_SIZE-1-1)) {
        //sum[i+j+1] = sum[i+j+1] + (par_res[1] >> (VAR_SIZE/2)) + (par_res[2] >> (VAR_SIZE/2)) + par_res[3];
        carry[i+j+2] += sum_4_mul(&sum[i+j+1],(par_res[1] >> (VAR_SIZE/2)),&carry[i+j+1],1);
        carry[i+j+2] += sum_4_mul(&sum[i+j+1],(par_res[2] >> (VAR_SIZE/2)),&carry[i+j+1],0);
        carry[i+j+2] += sum_4_mul(&sum[i+j+1],par_res[3],&carry[i+j+1],0);
      }
    }
  }

  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    data_res.numb[i] = sum[i];
    //printf("stage %d mul = %x\n", i, data_res.numb[i]);
  }
  data_res.numb[i] = sum[i];

  data_res.pos = INT_SIZE + VAR_SIZE*!(data_res.numb[NUMB_SIZE-1] == 0);
  //printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[NUMB_SIZE-1]);


  return data_res;
}
/*
int main() {

  bigint_t a, b, res;
  int i;
  a.numb[0] = 0x12345678;
  b.numb[0] = UMAX;
  for (i = 1; i < NUMB_SIZE - 2; i ++) {
    a.numb[i] = 0x00000001;
    b.numb[i] = 0;
  }

    a.numb[i] = 0;
    b.numb[i] = 0;

  res = sum(a,b);
  res = sub(a,b);
  res = mul(a,b);

  return 0;
}
*/

int main(int argc, char **argv){

  int i, max_i, j;
  bigint_t a, b, res;
  char temp[HEX_DIGIT];
  int res_logic, shift;
  int res_shift;


  if (argc != 4){
    printf("Error, <command> <operation> <operanda> <operandb>\n");
    return 1;
  }

  // get data from line
  max_i = NUMB_SIZE - 1;

  if ((!strcmp(argv[1],"sum")) || !strcmp(argv[1],"sub") || !strcmp(argv[1],"mul")) {
    for (i=max_i-1;i>=0;i--){
      strncpy(temp,argv[2]+2+HEX_DIGIT*(max_i-1-i),HEX_DIGIT);
      a.numb[i] = (var_t)strtol(temp, NULL, 16);
      strncpy(temp,argv[3]+2+HEX_DIGIT*(max_i-1-i),HEX_DIGIT);
      b.numb[i] = (var_t)strtol(temp, NULL, 16);
    }
    a.pos=0;
    b.pos=0;
  } else if ((!strcmp(argv[1],"lsl")) || (!strcmp(argv[1],"lsr"))) {
    for (i=max_i;i>=0;i--){
      strncpy(temp,argv[2]+2+HEX_DIGIT*(max_i-i),HEX_DIGIT);
      a.numb[i] = (var_t)strtol(temp, NULL, 16);
    }
    shift=atoi(argv[3]);
  } else {
    for (i=max_i;i>=0;i--){
      strncpy(temp,argv[2]+2+HEX_DIGIT*(max_i-i),HEX_DIGIT);
      a.numb[i] = (var_t)strtol(temp, NULL, 16);
      strncpy(temp,argv[3]+2+HEX_DIGIT*(max_i-i),HEX_DIGIT);
      b.numb[i] = (var_t)strtol(temp, NULL, 16);
    }
    a.pos=0;
    b.pos=0;
  }


/*
  for (i=0;i<max_i;i++){
    printf("a %d = %x\n",i,a.numb[i]);
    printf("b %d = %x\n",i,b.numb[i]);
  }
*/

  if (!strcmp(argv[1],"sum")){
    res = sum(a,b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"sub")) {
    res = sub(a,b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"mul")) {
    res = mul(a,b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"lsl")) {
    res_shift = lsl(&a,shift);
    print_to_stdout(&a);
  } else if (!strcmp(argv[1],"lsr")) {
    res_shift = lsr(&a,shift);
    print_to_stdout(&a);
  } else if (!strcmp(argv[1],"eq")) {
    res_logic = eq(a,b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"df")) {
    res_logic = df(a,b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"ge")) {
    res_logic = ge(a,b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"gt")) {
    res_logic = gt(a,b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"le")) {
    res_logic = le(a,b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"lt")) {
    res_logic = lt(a,b);
    printf("%d\n", res_logic);
  } else {
    printf("No operation available");
  }


  return 0;
}
