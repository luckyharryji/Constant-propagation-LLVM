#include <stdio.h>
#include <CAT.h>

void CAT_execution (CATData d1){
	CATData	d2,d3;

  d2	= CAT_create_signed_value(8);
	CAT_binary_add(d1, d2, d2);
	printf("H1: 	Value 1 = %ld\n", CAT_get_signed_value(d1));
	printf("H1: 	Value 2 = %ld\n", CAT_get_signed_value(d2));

	return ;
}

int main (int argc, char *argv[]){
  CATData d1;
	d1	= CAT_create_signed_value(5);
	CAT_execution(d1);

	return 0;
}
