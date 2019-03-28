#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "bigint.h"
#include "mm.h"

int main(int argc, char **argv){

  int i, nb;
  bigint_t a, b, n, res;
  char temp[HEX_DIGIT];
  int res_logic, shift;
  int res_shift;


  if (argc > 5){
    printf("Error, <command> <operation> <operanda> <operandb> <operandb>\n");
    return 1;
  }


  if ((!strcmp(argv[1],"sum")) || !strcmp(argv[1],"sub") || !strcmp(argv[1],"mul")) {
    a = init(argv[2]);
    b = init(argv[3]);
  } else if ((!strcmp(argv[1],"lsl")) || (!strcmp(argv[1],"lsr"))) {
    a = init(argv[2]);
    shift=atoi(argv[3]);
  } else if ((!strcmp(argv[1],"mm")) || (!strcmp(argv[1],"me"))) {
    a = init(argv[2]);
    b = init(argv[3]);
    n = init(argv[4]);
    nb = INT_SIZE;
  } else {
    a = init(argv[2]);
    b = init(argv[3]);
  }


  if (!strcmp(argv[1],"sum")){
    res = sum(&a,&b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"sub")) {
    res = sub(&a,&b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"mul")) {
    res = mul(&a,&b);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"lsl")) {
    res = lsl(&a,shift);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"lsr")) {
    res = lsr(&a,shift);
    print_to_stdout(&res);
  } else if (!strcmp(argv[1],"eq")) {
    res_logic = eq(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"df")) {
    res_logic = df(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"ge")) {
    res_logic = ge(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"gt")) {
    res_logic = gt(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"le")) {
    res_logic = le(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"lt")) {
    res_logic = lt(&a,&b);
    printf("%d\n", res_logic);
  } else if (!strcmp(argv[1],"mm")) {
    res = MM_big(&a,&b,&n,nb);
    print_to_stdout(&res);
  } else {
    printf("No operation available");
  }


  return 0;
}