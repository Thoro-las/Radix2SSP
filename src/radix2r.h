#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.h"

// Add a zero to LSB and pad with zeros to multiple of window
uint32_t pad(uint32_t number, int window, int* length) {
  uint32_t padded = number << 1;
  int newlength = bitlength(number);
  newlength += window - newlength % window;
  *length = newlength;
  return padded;
}

// Extract index-th slice of size window + 1 starting at LSB and turn it into two's complement
int slice(uint32_t number, int window, int index) {
  uint32_t slice = extract(number, index * window, window + 1);
  int msv = (slice & (((1 << window) - 1) << 1)) >> 1;
  int sign = (slice >> window) & 0b1;
  int lsb = slice & 0b1;

  return msv + lsb - sign * (1 << window);
}

// Returns the largest power of 2 that divides the number 
uint32_t factor2(uint32_t number) {
  int pow2 = 0;

  while (number % 2 == 0 && number > 0) {
    number = number / 2;
    pow2++;
  }

  return pow2;
}

// Radix-2r representation of a number with window size 'window' DYNAMIC MEMORY ALLOCATION MUST FREE
char* radix2r(uint32_t number, int window) {
  int length = bitlength(number);
  int groups = (length) / window;
  char* representation = (char*) malloc(sizeof(char) * length);

  for (int i = 0; i < groups; i++) {
    int Qi = slice(number, window, i);

    int si = Qi >= 0 ? 0 : 1;
    int ni = factor2(Qi);
    int mi = (int) abs(Qi) / pow(2, ni);

    for (int j = window - 1; j >= 0; j--) {
      int position = window*(groups - i - 1) + j;
      char c = si == 0 ? (char) ('0' + mi) : (char) ('A' + mi);
      representation[position] = (window - j - 1) == ni ? c : '0';
    }
  }

  representation[length] = '\0';
  return representation;
}
