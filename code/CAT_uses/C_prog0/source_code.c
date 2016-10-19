#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double computation_intensive (double seed, int num){
  for (int j=0; j < num; j++){
    for (int z=0; z < num; z++){
      seed = seed / seed;
      seed = (seed + 1000) * 1.5;
    }
  }

  return seed;
}

int main (int argc, char *argv[]){
  double v;
  double *seeds;
  double seed;
  int iters;

  if (argc < 3){
    printf("USAGE: %s ITERATION_NUMBER SEED\n", argv[0]);
    return 1;
  }
  iters = atoi(argv[1]);
  seed = atof(argv[2]);
  v = 0;

  fprintf(stderr, "|");
  for (int i=0; i < 100; i++){
    v += computation_intensive(seed, iters);
    fprintf(stderr, "|");
  }

  fprintf(stderr, "\n%f\n", v);

  return 0;
}
