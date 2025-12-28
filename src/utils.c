#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Converts a number to a binary string of length 'length' DYNAMIC MEMORY
// ALLOCATION MUST FREE
char *tobinaryl(uint32_t number, int length, int* ones) {
  *ones = 0;
  if (length > 32)
    length = 32;

  char *buffer = (char *)malloc(sizeof(char) * (length + 1));
  for (int i = 0; i < length; i++) {
    if ((number >> i) & 0b1) (*ones)++;
    buffer[length - 1 - i] = (number >> i) & 0b1 ? '1' : '0';
  }

  buffer[length] = '\0';
  return buffer;
}

// Converts a number to a binary string of length 'length' with spaces every
// 'window' bits DYNAMIC MEMORY ALLOCATION MUST FREE
char *tobinarylw(uint32_t number, int length, int window) {
  length = length + window - (length - 1) % window;
  int spacedlength = length + (int)(length / window) - 1;
  int cursor = 0;
  int ones = 0;

  char *buffer = tobinaryl(number, length, &ones);
  char *spacedbuffer = (char *)malloc(sizeof(char) * spacedlength);

  for (int i = 0; i < spacedlength; i++) {
    if (i % (window + 1) == window) {
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
char *tobinary(uint32_t number, int *ones) { return tobinaryl(number, 32, ones); }

// Extracts 'length' bits from the binary representation of 'number' starting at
// 'start' from LSB
uint32_t extract(uint32_t number, int start, int length) {
  return (number >> start) & ((1 << length) - 1);
}

// Returns the bit length of a number
int bitlength(uint32_t number) {
  int length = 0;
  while ((number >>= 1) > 0)
    length++;
  return length + 1;
}

struct Word {
  char *content;
  size_t length;

  struct Word *next;
};

struct Sentence {
  size_t length;
  struct Word *head;
  struct Word *tail;
};

typedef struct Word Word;
typedef struct Sentence Sentence;

Word *fromstring(char *string) {
  Word *word = (Word *)malloc(sizeof(Word));
  word->content = string;
  word->length = 0;
  while (string[word->length] != '\0')
    word->length++;
  return word;
}

Sentence *empty() {
  Sentence *sentence = (Sentence *)malloc(sizeof(Sentence));
  sentence->head = NULL;
  sentence->tail = NULL;
  sentence->length = 0;
  return sentence;
}

Sentence *prepend(Sentence *sentence, char *string) {
  if (!sentence || !string)
    return NULL;
  Word *word = fromstring(string);

  if (sentence->length == 0)
    sentence->tail = word;
  else
    word->next = sentence->head;

  sentence->head = word;
  sentence->length += word->length;

  return sentence;
}

Sentence *append(Sentence *sentence, char *string) {
  if (!sentence || !string)
    return NULL;
  Word *word = fromstring(string);

  if (sentence->length == 0)
    sentence->head = word;
  else
    sentence->tail->next = word;
  sentence->tail = word;
  sentence->length += word->length;

  return sentence;
}

void destroy(Sentence *sentence) {
  if (sentence->length == 0)
    return;

  Word *current = sentence->head;
  while (current != sentence->tail) {
    Word *next = current->next;
    free(current);
    current = next;
  }
  free(sentence->tail);
}

char *tostring(Sentence *sentence) {
  if (!sentence)
    return NULL;
  if (sentence->length == 0)
    return "\0";

  unsigned int position = 0;
  unsigned int currentposition = 0;

  char *contents = (char *)malloc(sizeof(char) * (sentence->length + 1));
  Word *current = sentence->head;

  while (position < sentence->length) {
    if (currentposition >= current->length) {
      current = current->next;
      currentposition = 0;
    }

    contents[position] = current->content[currentposition];
    currentposition++;
    position++;
  }

  destroy(sentence);
  return contents;
}
