#pragma once
#include "utils.c"
#include "string.h"

// Extract index-th slice of size window + 1 starting at LSB and turn it into two's complement
int sslice(uint32_t number, int window, int start) {
  uint32_t slice = extract(number, start, window);
  int value = slice & ((1 << window) - 1);
  int sign = (slice >> (window - 1)) & 0b1;

  return value - sign * (1 << window);
}

// Get position of next 1 from start position
int nextone(uint32_t number, int start) {
  int pos = start;
  while (extract(number, pos, 1) == 0 && pos < 32) pos++;
  return pos;
}

// SSP representation of a number with window size 'window' DYNAMIC MEMORY ALLOCATION
char* ssp(uint32_t number, int window, int* ones) {
  if (number == 0) return "0";
  int length = bitlength(number) + 1;

  int i = 0;
  *ones = 0;

  Sentence* representation = empty();

  while (i < length) {
    int j = nextone(number, i);
    for (int k = i; k < j; k++)
      prepend(representation, "0");
    i = j;

    int S = sslice(number, window, i);
    int s = S >= 0 ? 0 : 1;

    char* temp = (char*) malloc(window + 3);
    sprintf(temp, "(%c%d)", s == 0 ? '+' : '-', abs(S));
    prepend(representation, temp);
    (*ones)++;
    for (int j = 1; j < window; j++)
      prepend(representation, "0");
    i = i + window;

    if (s == 1) {
      while (extract(number, i, 1) == 1) {
        prepend(representation, "0");
        i++;
      }
      prepend(representation, "(+1)");
      (*ones)++;
      i++;
    }
  }

  destroy(representation);
  return tostring(representation);
} 
