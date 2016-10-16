#include <stdio.h>
#include "CAT.h"

int CAT_execution1 (void){
  CATData d1;
  CATData d2;
  CATData d3;
  d1  = CAT_create_signed_value(5);
  d2  = CAT_create_signed_value(8);
  d3  = CAT_create_signed_value(0);
  CAT_binary_add(d3, d1, d2);
  return CAT_get_signed_value(d3);
}

int CAT_execution2 (void){
  CATData d;
  d  = CAT_create_signed_value(5);
  return CAT_get_signed_value(d);
}

int main (int argc, char *argv[]){
  return CAT_execution1() + CAT_execution2();
}
