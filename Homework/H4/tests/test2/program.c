#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3,d4,d5;

	d1	= CAT_create_signed_value(5);
  d2	= CAT_create_signed_value(8);
  d3	= CAT_create_signed_value(8);
  d4	= CAT_create_signed_value(8);
  if (userInput > 10){
	  CAT_binary_add(d1, d2, d3);
  }
	printf("H1: 	Value 1 = %lld\n", CAT_get_signed_value(d1));
	printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));
	printf("H1: 	Value 3 = %lld\n", CAT_get_signed_value(d3));
	printf("H1: 	Value 4 = %lld\n", CAT_get_signed_value(d4));
	
  CAT_binary_add(d1, d2, d3);
	printf("H1: 	Value 1 = %lld\n", CAT_get_signed_value(d1));
	printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));
	printf("H1: 	Value 3 = %lld\n", CAT_get_signed_value(d3));
	printf("H1: 	Value 4 = %lld\n", CAT_get_signed_value(d4));

  if (userInput > 20){
	  d5	= CAT_create_signed_value(0);
  }
	printf("H1: 	Value 1 = %lld\n", CAT_get_signed_value(d1));
	printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));
	printf("H1: 	Value 3 = %lld\n", CAT_get_signed_value(d3));
	printf("H1: 	Value 4 = %lld\n", CAT_get_signed_value(d4));

  if (userInput > 25){
	  printf("H1: 	Value 5 = %lld\n", CAT_get_signed_value(d4));
  }

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

	return 0;
}
