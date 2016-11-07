#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData x,y;
  void **ref1;
  void **ref2;

  ref1 = malloc(sizeof(void *));
  ref2 = malloc(sizeof(void *));

  x	= CAT_create_signed_value(8);
  y = CAT_create_signed_value(42);

  (*ref1) = x;
  (*ref2) = y;

  CAT_binary_add(*ref2, x, y);

  printf("H1: 	X    = %ld\n", CAT_get_signed_value(x));
  printf("H1: 	Y    = %ld\n", CAT_get_signed_value(y));
  printf("H1: 	Ref1 = %ld\n", CAT_get_signed_value(*ref1));
  printf("H1: 	Ref2 = %ld\n", CAT_get_signed_value(*ref2));

  free(ref1);
  free(ref2);

  return 0;
}
