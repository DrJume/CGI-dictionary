/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

dictionary_t *addDictionaryEntry(dictionary_t *dict, size_t dict_len,
                                 dictionary_t *entry) {
  // Allocate memory for new dict struct array with one extra space
  dictionary_t *dict_new = calloc(dict_len + 1, sizeof(dictionary_t));

  // Copy old data into new array
  memcpy(dict_new, dict, dict_len * sizeof(dictionary_t));

  // Free old dict memory space
  free(dict);

  // Copy entry data into new data array at last position
  memcpy(dict_new + dict_len, entry, sizeof(dictionary_t));

  return dict_new;
}

dictionary_t *deleteDictionaryEntry(dictionary_t *dict, size_t dict_len,
                                    int index) {
  // Allocate memory for new dict struct array with one less space
  dictionary_t *dict_new = calloc(dict_len - 1, sizeof(dictionary_t));

  // Copy data until index into new array
  memcpy(dict_new, dict, index * sizeof(dictionary_t));

  // Copy data after index into new array
  memcpy(dict_new + index, dict + (index + 1),
         (dict_len - (index+1) ) * sizeof(dictionary_t));

  free(dict);

  return dict_new;
}

int sortByEnglish(const void *a, const void *b) {
  dictionary_t* entry_a = (dictionary_t*)a;
  dictionary_t* entry_b = (dictionary_t*)b;

  return strcasecmp(entry_a->english, entry_b->english);
}

int sortByDeutsch(const void *a, const void *b) {
  dictionary_t *entry_a = (dictionary_t *)a;
  dictionary_t *entry_b = (dictionary_t *)b;

  return strcasecmp(entry_a->deutsch, entry_b->deutsch);
}

void sortDictionary(dictionary_t *dict, size_t dict_len, char* sortBy) {

  int (*sortFunc) (const void *p1, const void *p2);

  if(strcmp(sortBy, "english") == 0) {
    sortFunc = sortByEnglish;
  } else if (strcmp(sortBy, "deutsch") == 0) {
    sortFunc = sortByDeutsch;
  }

  qsort(dict, dict_len, sizeof(dictionary_t), sortFunc);
}