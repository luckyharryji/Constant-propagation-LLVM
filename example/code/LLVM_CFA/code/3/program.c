#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
  uint64_t iters;
  double v;

  if (argc < 3){
    printf("USAGE: %s ITERATIONS VALUE\n", argv[0]);
    return 1;
  }
  iters = atoll(argv[1]);
  v     = atof(argv[2]);

  for (uint64_t i=0; i < iters; i++){
    for (uint64_t j=0; j < iters; j++){
      v = sqrt(v);
    }
  }

  if (v < 0.1){ 
    return 2;
  }
  return 0;
}
