#include "../include/bigint.h"




// Return 0 if different, else 1; -1 if not aligned
int eq(bign_t first, bign_t second){
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
int df(bign_t first, bign_t second){
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
int gt(bign_t first, bign_t second){
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
int ge(bign_t first, bign_t second){
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
int lt(bign_t first, bign_t second){
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
int le(bign_t first, bign_t second){
    if (first.pos != second.pos){
        return -1;
    }
    for(int i=NUMB_SIZE; i>0; i--){
        if(first.numb[i] > second.numb[i])
           return 0;
        else if(first.numb[i] < second.numb[i])
           return 1;
    };
    return 1;
};



