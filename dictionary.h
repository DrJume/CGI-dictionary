/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#ifndef DICTIONARY_T_H
#define DICTIONARY_T_H

#include <string.h>

#define DICT_FILE_MAX_LINE_LEN 128
#define DICT_MAX_STRLEN 60

typedef struct dictionary_t {
  int index;
  char english[DICT_MAX_STRLEN + 1];
  char deutsch[DICT_MAX_STRLEN + 1];
} dictionary_t;

dictionary_t *addDictionaryEntry(dictionary_t *dict, size_t dict_len,
                                 dictionary_t *entry);

dictionary_t *deleteDictionaryEntry(dictionary_t *dict, size_t dict_len,
                                    int index);

void sortDictionary(dictionary_t *dict, size_t dict_len, char* sortBy);

#endif
