#include <stdio.h>
#include <CAT.h>

int another_execution (int userInput, CATData y){
	printf("H1: 	Y = %ld\n", CAT_get_signed_value(y));
	CAT_binary_add(y, y, y);
	return CAT_get_signed_value(y);
}

void CAT_execution (int userInput, CATData d1, int userInput2){
	CATData d2,d3;

  d2	= CAT_create_signed_value(8);
	printf("H1: 	Value 2 = %ld\n", CAT_get_signed_value(d2));
  if (userInput > 10){
    d2	= CAT_create_signed_value(8);
	  CAT_binary_add(d1, d2, d2);
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get_signed_value(d2));
	printf("H1: 	Value 1 = %ld\n", CAT_get_signed_value(d1));
  if (userInput2 > 0){
	  printf("H1: 	Value 2 = %ld\n", CAT_get_signed_value(d2));
  }

	return ;
}

int main (int argc, char *argv[]){
  CATData x;

  x	= CAT_create_signed_value(8);
	CAT_execution(argc, x, argc+1);

	printf("Result 2 = %d\n", another_execution(argc, x));

	return 0;
}
