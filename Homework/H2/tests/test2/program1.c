#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3,d4,d5,d6,d7;

	d1	= CAT_create_signed_value(5);
	printf("H1: 	Value 1 = %lld\n", CAT_get_signed_value(d1));

  d2	= CAT_create_signed_value(8);
  if (userInput > 10){
	  CAT_binary_add(d2, d2, d2);
  }
	printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));

	d3	= CAT_create_signed_value(0);
	CAT_binary_add(d3, d1, d2);
	
  d4	= CAT_create_signed_value(0);
	CAT_binary_add(d4, d1, d3);
	
  d5	= CAT_create_signed_value(0);
	CAT_binary_add(d5, d3, d4);
	
  d6	= CAT_create_signed_value(0);
	d7	= CAT_create_signed_value(0);

	CAT_binary_add(d6, d1, d1);
	CAT_binary_add(d7, d1, d1);
	printf("H1: 	Result = %lld\n", CAT_get_signed_value(d7));

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

	return 0;
}
