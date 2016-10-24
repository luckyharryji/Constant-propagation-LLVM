#include <stdio.h>

int f (int p1){
  int v = p1 * 3;

  if (p1 > 5){
    v = 1;
  } else {
    v *= 3;
  }

  return v;
}

int main (int argc, char *argv[]) {
  int v;

  v = f(argc);

  return v;
}
