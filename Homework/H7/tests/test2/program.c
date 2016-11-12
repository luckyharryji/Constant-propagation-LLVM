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

CATData q2 (int valueQ){
  return p(valueQ + 8);
}

CATData q3 (int valueQ){
  return p(valueQ + 1);
}

int main (int argc, char *argv[]){
  CATData r = q1(1);
  CATData t = q2(3);
  CATData z = q3(1);

  printf("%lld %lld %lld\n", CAT_get_signed_value(r), CAT_get_signed_value(t), CAT_get_signed_value(z));

	return 0;
}
