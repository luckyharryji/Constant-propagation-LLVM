#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData x, y;
  void **mem;

  mem = malloc(sizeof(void *));

  x	= CAT_create_signed_value(8);
  (*mem) = x;

  y = (*mem);

	printf("H1: 	Y = %ld\n", CAT_get_signed_value(y));
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));

  CAT_binary_add(*mem, *mem, *mem);
	printf("H1: 	Y = %ld\n", CAT_get_signed_value(y));
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));

	return 0;
}
