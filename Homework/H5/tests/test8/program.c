#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

CATData * function_that_complicates_everything (int argc, CATData *par1, CATData *par2){
  if (argc > 5){
    return par1;
  }
  return par2;
}

int main (int argc, char *argv[]){
  CATData d1,d2;
  CATData *dp;
  int64_t value, value1, value2;

	d1	= CAT_create_signed_value(5);
	d2	= CAT_create_signed_value(7);
  
  dp = function_that_complicates_everything(argc, &d1, &d2);

  value = CAT_get_signed_value(*dp);
  value1 = CAT_get_signed_value(d1);
  value2 = CAT_get_signed_value(d2);

  printf("Values: %ld %ld %ld\n", value, value1, value2);

	return 0;
}
