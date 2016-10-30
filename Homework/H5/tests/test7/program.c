#include <stdio.h>
#include <CAT.h>

int main (int argc, char *argv[]){
  CATData d1;
  
  if (argc > 10){
    d1	= CAT_create_signed_value(8);
  } else {
    d1	= CAT_create_signed_value(8);
  }

	printf("H5: 	Value of d1 = %ld\n", CAT_get_signed_value(d1));

	return 0;
}
