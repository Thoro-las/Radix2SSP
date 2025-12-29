#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.c"

// Add a zero to LSB and pad with zeros to multiple of window
uint64_t pad(uint64_t number, int window) {
  return number << 1;
}

// Extract index-th slice of size window + 1 starting at LSB and turn it into two's complement
int rslice(uint64_t number, int window, int index) {
  uint64_t slice = extract(number, index * window, window + 1);
  int msv = (slice & (((1 << window) - 1) << 1)) >> 1;
  int sign = (slice >> window) & 0b1;
  int lsb = slice & 0b1;

  return msv + lsb - sign * (1 << window);
}

// Returns the largest power of 2 that divides the number 
uint64_t factor2(uint64_t number) {
  int pow2 = 0;

  while (number % 2 == 0 && number > 0) {
    number = number / 2;
    pow2++;
  }

  return pow2;
}

// Radix-2r representation of a number with window size 'window' DYNAMIC MEMORY ALLOCATION MUST FREE
char* radix2r(uint64_t number, int window, int* ones) {
  number = number << 1;
  *ones = 0;

  if (number == 0) {
    char *z = (char*) malloc(2);
    z[0] = '0';
    z[1] = '\0';
    *ones = 1;
    return z;
  }

  int length = bitlength(number);
  int groups = (int) ceil((float) length / window);
  Sentence* representation = empty();

  for (int i = 0; i < groups; i++) {
    int Qi = rslice(number, window, i);

    int si = Qi >= 0 ? 0 : 1;
    int ni = factor2(abs(Qi));
    int mi = abs(Qi) >> ni;

    for (int j = 0; j < window; j++) {
      if (j != ni || Qi == 0) {
        prepend(representation, "0");
      } else {
        char* temp = (char*) malloc(window + 3);
        sprintf(temp, "(%c%d)", si == 0 ? '+' : '-', mi);
        prepend(representation, temp);
        free(temp);
        (*ones)++;
      }
    }
  }

  char* out = tostring(representation);
  destroy(representation);
  return out;
}
