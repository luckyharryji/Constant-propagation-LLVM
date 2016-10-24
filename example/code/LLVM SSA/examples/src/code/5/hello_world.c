#include <stdio.h>

int main (int argc, char *argv[]) {
  int v,x;

  v = argc + 5;
  if (argc > 3){
    v += 42;
  }
  x = v * 3;

  return x;
}
