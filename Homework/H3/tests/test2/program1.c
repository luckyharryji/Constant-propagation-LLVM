#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3;

	d1	= CAT_create_signed_value(5);
	printf("H1: 	Value 1 = %lld\n", CAT_get_signed_value(d1));

  for (int i=0; i < userInput; i++){
    d2	= CAT_create_signed_value(8);
    if (userInput > 10){
	    CAT_binary_add(d2, d2, d2);
    }
	  printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));

	  d3	= CAT_create_signed_value(0);
	  CAT_binary_add(d3, d1, d2);
	  CAT_binary_add(d3, d1, d3);
	  CAT_binary_add(d3, d3, d3);
    if (userInput > 20){
	    CAT_binary_add(d3, d1, d1);
    }
	  printf("H1: 	Result = %lld\n", CAT_get_signed_value(d3));
  }

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

	return 0;
}
