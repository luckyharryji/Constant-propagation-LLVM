#include <stdio.h>

int main (int argc, char *argv[]) {
  int v;

  v = argc;

  if (argc > 5){
    v = 1;
  } else {
    v *= 3;
  }

  return v;
}
