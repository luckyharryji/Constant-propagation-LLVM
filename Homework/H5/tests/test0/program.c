#include <stdio.h>
#include <CAT.h>

void CAT_execution (CATData	d1){
	CATData d2,d3;

  d2	= CAT_create_signed_value(8);
	CAT_binary_add(d1, d2, d2);
	printf("H1: 	Value 2 = %lld\n", CAT_get_signed_value(d2));

	d3	= CAT_create_signed_value(0);
	CAT_binary_add(d3, d1, d2);
	printf("H1: 	Result = %lld\n", CAT_get_signed_value(d3));

	return ;
}

int main (int argc, char *argv[]){
  CATData d1;
	d1	= CAT_create_signed_value(5);
	CAT_execution(d1);

	return 0;
}
