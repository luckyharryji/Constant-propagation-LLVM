#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"

CATData p (int value){
  if (value > 10){
    return CAT_create_signed_value(5);
  }

  return CAT_create_signed_value(6);
}

CATData q1 (int valueQ){
  return p(1);
}


int main (int argc, char *argv[]){
  CATData r = q1(1);

  printf("%lld\n", CAT_get_signed_value(r));

	return 0;
}
