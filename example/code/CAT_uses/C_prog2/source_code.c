#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double computation_intensive (double *seeds, int num){
  double tot;

  for (int j=0; j < num; j++){
    for (int z=0; z < num; z++){
      seeds[j] = seeds[j] / seeds[j];
      seeds[j] = (seeds[j] + 1000) * 1.5;
    }
  }

  tot = 0;
  for (int j=0; j < num; j++){
    tot += seeds[j];
  }

  return tot;
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
  seeds = malloc(sizeof(double) * iters);
  for (int i=0; i < iters; i++){
    seeds[i] = seed;
  }

  fprintf(stderr, "|");
  for (int i=0; i < 100; i++){
    v += computation_intensive(seeds, iters);
    fprintf(stderr, "|");
  }

  fprintf(stderr, "\n");

  return 0;
}
