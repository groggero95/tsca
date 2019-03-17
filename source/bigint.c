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
};

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
};

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
    };
    return 0;
};

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
    };
    return 1;
};


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
    };
    return 0;
};

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
};


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

  var_t carry = 0;
  var_t i;

  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    data_res.numb[i] = a.numb[i] + b.numb[i] + carry;
    printf("stage %d sum = %x\n", i, data_res.numb[i]);
    if (data_res.numb[i] < a.numb[i]){
      carry = 1;
    } else {
      carry = 0;
    }
  }

  data_res.pos = INT_SIZE + carry*VAR_SIZE;
  data_res.numb[i] = carry;
  printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);
  return data_res;
}

// SUB, gets data on NUMB_SIZE -1  and returns data on NUMB_SIZE
bigint_t sub(bigint_t a, bigint_t b){

  bigint_t data_res;

  var_t carry = 1;
  var_t i, k, j;

  for (i = 0; i < NUMB_SIZE-1 ; i++) {
    b.numb[i] = b.numb[i]^UMAX;
    data_res.numb[i] = a.numb[i] + b.numb[i] + carry;
    printf("stage %d sub = %x", i, data_res.numb[i]);
    if (data_res.numb[i] <= a.numb[i]){
      carry = 1;
    } else {
      carry = 0;
    }
    printf(" with next carry %x\n",carry);
  }

  data_res.pos = INT_SIZE + !carry*VAR_SIZE;
  data_res.numb[i] = 0-!carry;
  printf("and additional digits pos = %d, last = %x\n",data_res.pos,data_res.numb[i]);
  return data_res;
}
