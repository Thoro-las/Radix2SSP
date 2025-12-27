#pragma once
#include "utils.h"
#include "string.h"

// Extract index-th slice of size window + 1 starting at LSB and turn it into two's complement
int slice(uint32_t number, int window, int start) {
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
char* ssp(uint32_t number, int window) {
  if (number == 0) return "0";

  int length = bitlength(number) + 1;
  int i = 0;

  char* representation = (char*) malloc(sizeof(char) * (length + 1));
  
  while (i < length) {
    int j = nextone(number, i);
    for (int k = i; k < j; k++)
      representation[length - k - 1] = '0';
    i = j;

    int S = slice(number, window, i);
    int s = S >= 0 ? 0 : 1;
    
    for (int j = 1; j < window; j++)
      representation[(length - i - 1) - j] = '0';
    
    representation[length - i - 1] = s == 0 ? (char) '0' + S : (char) 'A' - S;
    i = i + window;

    if (s == 1) {
      while (extract(number, i, 1) == 1) {
        representation[length - i - 1] = '0';
        i++;
      }
      representation[length - i - 1] = '1';
      i++;
    }
  }

  representation[length + 1] = '\0';
  return representation;
} 