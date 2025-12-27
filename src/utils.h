#pragma once

#include <stdint.h>
#include <stdlib.h>

// Converts a number to a binary string of length 'length' DYNAMIC MEMORY ALLOCATION MUST FREE
char *tobinaryl(uint32_t number, int length) {
  if (length > 32) length = 32;

  char *buffer = (char*) malloc(sizeof(char) * (length + 1));
  for (int i = 0; i < length; i++)
    buffer[length - 1 - i] = (number >> i) & 0b1 ? '1' : '0';

  buffer[length] = '\0';
  return buffer;
}

// Converts a number to a binary string of length 'length' with spaces every 'window' bits DYNAMIC MEMORY ALLOCATION MUST FREE
char *tobinarylw(uint32_t number, int length, int window) {
  length = length + window - (length - 1) % window;
  int spacedlength = length + (int) (length / window) - 1;
  int cursor = 0;

  char *buffer = tobinaryl(number, length);
  char *spacedbuffer = (char*) malloc(sizeof(char) * spacedlength);

  for (int i = 0; i < spacedlength; i++) {
    if (i % (window+1) == window) {
      spacedbuffer[i] = ' ';
      continue;
    }

    spacedbuffer[i] = buffer[cursor];
    cursor++;
  }

  spacedbuffer[spacedlength] = '\0';
  return spacedbuffer;
}

// Converts a number to binary string of length 32
char *tobinary(uint32_t number) {
  return tobinaryl(number, 32);
}

// Extracts 'length' bits from the binary representation of 'number' starting at 'start' from LSB 
uint32_t extract(uint32_t number, int start, int length) {
  return (number >> start) & ((1 << length) - 1);
}

// Returns the bit length of a number
int bitlength(uint32_t number) {
  int length = 0;
  while ((number >>= 1) > 0) length++;
  return length + 1;
}
