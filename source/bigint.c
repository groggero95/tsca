#include "../include/bigint.h"


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
    for(int i=NUMB_SIZE; i>0; i--){
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
    for(int i=NUMB_SIZE; i>0; i--){
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
    for(int i=NUMB_SIZE; i>0; i--){
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
    for(int i=NUMB_SIZE; i>0; i--){
        if(first.numb[i] > second.numb[i])
            return 0;
        else if(first.numb[i] < second.numb[i])
            return 1;
        }
    return 1;
};


bigint_t sll(bigint_t number, int value){
    int full_shift = (int)value / VAR_SIZE;
        int rem_shift = (int) value % VAR_SIZE;
        bigint_t shifted;
        if(full_shift > 0){
            for(int i=0; i<NUMB_SIZE; i++){
                if(i>full_shift)
                    shifted.numb[i] = number.numb[i-full_shift];
                else
                    shifted.numb[i] = 0;
        }
        }

    int bit_temp;
    for(int j=0; j<NUMB_SIZE; j++){

    }


    //Update the position field
    shifted.pos = number.pos + value;
    return shifted;
}
