/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <string.h>

#include "dictionary.h"

dictionary_t *parseDictionaryFile(char *dict_file, size_t *dict_len_ret);

int saveDictionaryToFile(char *dict_file, dictionary_t *dict, size_t dict_len);
