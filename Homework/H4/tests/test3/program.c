#include <stdio.h>
#include "CAT.h"

int main (int argc, char *argv[]){
  CATData d = CAT_create_signed_value(argc);
  printf("%lld\n", CAT_get_signed_value(d));

  return 0;
}
