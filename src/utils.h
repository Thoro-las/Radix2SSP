#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  if (!string)
    return NULL;
  Word *word = (Word *)malloc(sizeof(Word));
  if (!word)
    return NULL;
  word->length = strlen(string);
  word->content = (char*) malloc(sizeof(char) * (word->length + 1));
  memcpy(word->content, string, word->length + 1);
  word->next = NULL;
  return word;
}

Sentence *empty() {
  Sentence *sentence = (Sentence *)malloc(sizeof(Sentence));
  if (!sentence)
    return NULL;
  sentence->head = NULL;
  sentence->tail = NULL;
  sentence->length = 0;
  return sentence;
}

Sentence *prepend(Sentence *sentence, char *string) {
  if (!sentence || !string)
    return sentence;
  Word *word = fromstring(string);
  if (!word)
    return NULL;
   
  word->next = sentence->head;
  sentence->head = word;
  if (sentence->tail == NULL) sentence->tail = word;  
  sentence->length += word->length;

  return sentence;
}

Sentence *append(Sentence *sentence, char *string) {
  if (!sentence || !string)
    return NULL;
  Word *word = fromstring(string);
  if (!word)
    return sentence;

  if (sentence->tail == NULL) {
    sentence->head = word;
    sentence->tail = word;  
  } else {
    sentence->tail->next = word;
    sentence->tail = word;
  }
  sentence->length += word->length;

  return sentence;
}

void destroy(Sentence *sentence) {
  if (!sentence)
    return;

  Word *current = sentence->head;
  while (current) {
    Word *next = current->next;
    free(current->content);
    free(current);
    current = next;
  }
  free(sentence);
}

char *tostring(Sentence *sentence) {
  if (!sentence)
    return NULL;
  if (sentence->length == 0) {
    char *s = malloc(1);
    if (s) s[0] = '\0';
    return s;
  }

  char *contents = (char *)malloc(sizeof(char) * (sentence->length + 1));
  
  if (!contents)
    return NULL;

  unsigned int position = 0;
  Word *current = sentence->head;
  unsigned int currentposition = 0;

  while (current && position < sentence->length) {
    if (currentposition >= current->length) {
      current = current->next;
      currentposition = 0;
      continue;
    }
    contents[position++] = current->content[currentposition++];
  }

  contents[sentence->length] = '\0';
  return contents;
}
