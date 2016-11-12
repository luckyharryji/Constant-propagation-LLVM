#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"

CATData p (void){
  return CAT_create_signed_value(1);
}

int main (int argc, char *argv[]){
  CATData r = p();
  
  CATData t = p();

  printf("%lld %lld\n", CAT_get_signed_value(r), CAT_get_signed_value(t));

	return 0;
}
