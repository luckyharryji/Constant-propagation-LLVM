#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

void function_that_complicates_everything (CATData *par1){
  CAT_binary_add(*par1, *par1, *par1);
  return ;
}

int main (int argc, char *argv[]){
	CATData d1	= CAT_create_signed_value(5);
  
  function_that_complicates_everything(&d1);

  int64_t value = CAT_get_signed_value(d1);

  printf("Values: %lld\n", value);

	return 0;
}
