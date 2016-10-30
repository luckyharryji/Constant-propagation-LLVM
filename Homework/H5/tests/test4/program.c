#include <stdio.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData x;
  x	= CAT_create_signed_value(8);
  x	= CAT_create_signed_value(9);
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));
  CAT_binary_add(x, x, x);
	printf("H1: 	X = %ld\n", CAT_get_signed_value(x));

	return 0;
}
