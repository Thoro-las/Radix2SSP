#include <stdio.h>
#include "utils.h"
// #include "radix2r.h"
#include "ssp.h"

int main() {

  int i = 0b11111101000111;
  int w = 4;
  int l = bitlength(i);

  printf("i: %s, ssp: %s\n", tobinaryl(i, 16), ssp(i, 4));
  
}
  