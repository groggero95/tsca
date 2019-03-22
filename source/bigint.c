#include "../include/bigint.h"
#include <stdio.h>


// Return 0 if different, else 1; -1 if not aligned
int eq(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=0; i<NUMB_SIZE; i++){
        if(first.numb[i]!=second.numb[i])
           return 0;
    }
    return 1;
}

// Return 0 if equal, else 1; -1 if not aligned
int df(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=0; i<NUMB_SIZE; i++){
        if(first.numb[i]!=second.numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if strictly gt, else 0; -1 if not aligned
int gt(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first.numb[i] < second.numb[i])
           return 0;
        else if(first.numb[i] > second.numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if ge, else 0; -1 if not aligned
int ge(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first.numb[i]<second.numb[i])
           return 0;
        else if(first.numb[i] > second.numb[i])
           return 1;
    }
    return 1;
}


// Return 1 if strictly lt, else 0; -1 if not aligned
int lt(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first.numb[i] > second.numb[i])
           return 0;
        else if(first.numb[i] < second.numb[i])
           return 1;
    }
    return 0;
}

// Return 1 if le, else 0; -1 if not aligned
int le(bigint_t first, bigint_t second){
    if (first.pos != second.pos){
            return -1;
    }
    for(int i=NUMB_SIZE-1; i>=0; i--){
        if(first.numb[i] > second.numb[i])
            return 0;
        else if(first.numb[i] < second.numb[i])
            return 1;
        }
    return 1;
}


// Logically shift right a, by pl places
int lsr(bigint_t *a, int pl){
	var_t tmp;
    int full_shift=pl/VAR_SIZE;
    pl = pl % VAR_SIZE;
    if (full_shift){
        //Full shift of VAR_SIZE*full_shift bits done
        for(int k=full_shift; k<NUMB_SIZE; k++){
            a->numb[k-full_shift]=a->numb[k];
            if (k>NUMB_SIZE-1-full_shift){
                a->numb[k]=0;
            }
        }
    }

	for (int i=0; i<NUMB_SIZE-1; i++){
		a->numb[i] >>= pl;
		tmp = a->numb[i+1];
		tmp <<= (VAR_SIZE-pl);
		a->numb[i] = a->numb[i] | tmp;
	}

	a->numb[NUMB_SIZE-1] >>= pl;
    return 1;
}

// Logically shift left a, by pl places
int lsl(bigint_t *a, int pl){
	var_t tmp;
    int full_shift=pl/VAR_SIZE;
    pl = pl % VAR_SIZE;
    if (full_shift){
        //Full shift of VAR_SIZE*full_shift bits done
        for(int k=NUMB_SIZE-1; k>=full_shift; k--){
            a->numb[k]=a->numb[k-full_shift];
            if (k-full_shift < full_shift){
                a->numb[k-full_shift]=0;
            }
        }
    }

	for (int i=NUMB_SIZE-1; i>0; i--){
    a->numb[i] <<= pl;
    tmp = a->numb[i-1];
    tmp >>= (VAR_SIZE-pl);
    a->numb[i] = a->numb[i] | tmp;
	}

    a->numb[0] <<= pl;
    return 1;
}

// Sum, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sum(bigint_t a, bigint_t b){

  bigint_t data_res;

  var_t carry = 0, new_carry = 0;
  int i;

  for (i = 0; i < NUMB_SIZE-1 ; i++){
    data_res.numb[i] = a.numb[i] + b.numb[i];
    new_carry = a.numb[i] > data_res.numb[i];
    data_res.numb[i] = data_res.numb[i] + carry;
    carry = new_carry | (a.numb[i] > data_res.numb[i]);
    printf("stage %d sum = %x\n", i, data_res.numb[i]);
  }

  data_res.pos = INT_SIZE + carry*VAR_SIZE;
  data_res.numb[i] = carry;
  printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);
  return data_res;
}

// SUB, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sub(bigint_t a, bigint_t b){

  bigint_t data_res;

  var_t borrow = 0, new_borrow=0;
  int i;

  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    data_res.numb[i] = a.numb[i] - b.numb[i];
    new_borrow = data_res.numb[i] > a.numb[i];
    data_res.numb[i] = data_res.numb[i] - borrow;
    borrow = new_borrow | (data_res.numb[i] > a.numb[i]);
    printf("stage %d sub = %x\n", i, data_res.numb[i]);
  }

  data_res.pos = INT_SIZE + borrow*VAR_SIZE;
  data_res.numb[i] = 0-borrow;
  printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);
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
bigint_t mul(bigint_t a, bigint_t b){

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
      par_res[0] = (b.numb[i]&LOMASK) * (a.numb[j]&LOMASK);
      par_res[1] = (b.numb[i]&LOMASK) * ((a.numb[j]&HIMASK) >> (VAR_SIZE/2));
      par_res[2] = ((b.numb[i]&HIMASK) >> (VAR_SIZE/2)) * (a.numb[j]&LOMASK);
      par_res[3] = ((b.numb[i]&HIMASK) >> (VAR_SIZE/2)) * ((a.numb[j]&HIMASK) >> (VAR_SIZE/2));

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
    printf("stage %d mul = %x\n", i, data_res.numb[i]);
  }
  data_res.numb[i] = sum[i];

  data_res.pos = INT_SIZE + VAR_SIZE*!(data_res.numb[NUMB_SIZE-1] == 0);
  printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[NUMB_SIZE-1]);
  return data_res;
}

int main(int argc, char **argv){


  if (argc != 2){
    printf("<command> <operation> <size> <operanda> <operandb>\n");
    return 1;
  }



  switch(argv[1]){
    case "add":
      res = add(argv[2],argv[3]);
      break;
    case "sub":
      res = sub(argv[2],argv[3]);
      break;
    case "mul":
      res = mul(argv[2],argv[3]);
      break;
    default:
      printf("Error\n");
      return 1;
    break;
  }

  return 0;
}
