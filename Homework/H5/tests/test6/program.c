#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData x, y, z;
  void **mem;

  mem = malloc(sizeof(void *));

  z	= CAT_create_signed_value(2);
  x	= CAT_create_signed_value(8);
  (*mem) = x;

  y = (*mem);

	printf("H1: 	Y = %ld\n", CAT_get_signed_value(y));
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));
	printf("H1: 	Z = %ld\n", CAT_get_signed_value(z));

  CAT_binary_add(*mem, *mem, *mem);
	printf("H1: 	Y = %ld\n", CAT_get_signed_value(y));
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));
	printf("H1: 	Z = %ld\n", CAT_get_signed_value(z));

	return 0;
}
