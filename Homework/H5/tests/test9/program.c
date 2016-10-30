#include <stdio.h>
#include <CAT.h>

void a_generic_C_function (CATData d1, int argc){
  if (argc > 10){
    d1	= CAT_create_signed_value(8);
  }
	printf("H5: 	Value of d1 = %ld\n", CAT_get_signed_value(d1));
  return ;
}


int main (int argc, char *argv[]){
  a_generic_C_function(CAT_create_signed_value(5), argc);
	return 0;
}
