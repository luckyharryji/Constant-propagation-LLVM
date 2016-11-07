#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData x;
  void **ref;

  ref = malloc(sizeof(void *));

  x = CAT_create_signed_value(8);
  (*ref) = x;

  printf("%ld", CAT_get_signed_value(x));

  free(ref);
  return 0;
}
