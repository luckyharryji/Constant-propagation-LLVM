#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"

void p (uint64_t v, CATData d){
  CATData m;

  if (v == 0){
    return CAT_binary_add(d, d, CAT_create_signed_value(1));
  }

  CAT_binary_add(d, d, CAT_create_signed_value(v + 5));

  p(v - 1, d);
  return ;
}

int main (int argc, char *argv[]){
  CATData x = CAT_create_signed_value(7);
  for (int i=1; i <= 5; i++){
    p(i, x);
    printf("%lld\n", CAT_get_signed_value(x));
  }

	return 0;
}
