#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"

CATData p (CATData v){
  CATData m;

  if (CAT_get_signed_value(v) < 10){
    m = CAT_create_signed_value(1);
  } else {
    m = CAT_create_signed_value(2);
  }
  return m;
}

int main (int argc, char *argv[]){
  CATData x = CAT_create_signed_value(7);
  CATData r = p(x);
  
  CATData y = CAT_create_signed_value(80);
  CATData t = p(y);

  printf("%lld %lld\n", CAT_get_signed_value(r), CAT_get_signed_value(t));

	return 0;
}
