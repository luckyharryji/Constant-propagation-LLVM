#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int64_t generic_C_function (CATData y){
  CATData x;
  void **ref;

  ref = malloc(sizeof(void *));

  x = CAT_create_signed_value(8);
  (*ref) = x;

  if (CAT_get_signed_value(y) > 10){
    int64_t v = CAT_get_signed_value(x);
    free(ref);
    return (v * 51) / 2;
  }
  free(ref);

  return 0;
}

int main (int argc, char *argv[]){
  printf("%ld\n", generic_C_function(CAT_create_signed_value(12)));

  return 0;
}
