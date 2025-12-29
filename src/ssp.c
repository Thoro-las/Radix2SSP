#pragma once

#include "utils.c"

// Extract index-th slice of size window + 1 starting at LSB and turn it into
// two's complement
int sslice(uint64_t number, int window, int start) {
  uint64_t slice = extract(number, start, window);
  int value = slice & ((1 << window) - 1);
  int sign = (slice >> (window - 1)) & 0b1;

  return value - sign * (1 << window);
}

// Get position of next 1 from start position
int nextone(uint64_t number, int start) {
  int pos = start;
  while (extract(number, pos, 1) == 0 && pos < 64) pos++;
  return pos;
}

// SSP representation of a number with window size 'window' DYNAMIC MEMORY
// ALLOCATION
char *ssp(uint64_t number, int window, int *ones) {
  if (number == 0) {
    char *z = (char *)malloc(2);
    z[0] = '0';
    z[1] = '\0';
    *ones = 1;
    return z;
  };

  int length = bitlength(number) + 1;
  int i = 0;
  *ones = 0;

  Sentence *representation = empty();

  while (i < length) {
    int j = nextone(number, i);
    for (int k = i; k < j; k++)
      prepend(representation, "0");
    i = j;

    int S = sslice(number, window, i);
    int s = S >= 0 ? 0 : 1;

    char *temp = (char *)malloc(window + 3);
    sprintf(temp, "(%c%d)", s == 0 ? '+' : '-', abs(S));
    prepend(representation, temp);
    free(temp);

    (*ones)++;
    for (int j = 1; j < window; j++)
      prepend(representation, "0");
    i += window;

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

  char *out = tostring(representation);
  destroy(representation);
  return out;
}
