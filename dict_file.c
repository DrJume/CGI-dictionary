/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

dictionary_t *parseDictionaryFile(char *dict_file, size_t *dict_len_ret) {
  // Open dictionary file
  FILE *fp = fopen(dict_file, "r");
  if (fp == NULL) {
    return NULL;
  }

  char vbuf[DICT_FILE_MAX_LINE_LEN];

  // Determine line count
  int lineCount = 0;
  while (fgets(vbuf, DICT_FILE_MAX_LINE_LEN, fp)) {
    lineCount++;
  }
  *dict_len_ret = lineCount;

  // Create dict array
  dictionary_t *dict = (dictionary_t *)calloc(lineCount, sizeof(dictionary_t));

  rewind(fp);
  for (int i = 0; i < lineCount; i++) {
    // Save index of entry
    dict[i].index = i;

    // Initialize char arrays with null bytes
    memset(dict[i].english, '\0', DICT_MAX_STRLEN + 1);
    memset(dict[i].deutsch, '\0', DICT_MAX_STRLEN + 1);

    // Read CSV line
    fgets(vbuf, DICT_FILE_MAX_LINE_LEN, fp);

    char *tok;
    if (vbuf[0] == ';') { // Issue with strtok when first column is empty ";..."
      strcpy(dict[i].english, "");

      // Cut ; away
      char vbuf_cut[strlen(vbuf) - 1];
      for (int i = 0; i < strlen(vbuf) - 1; i++) {
        vbuf_cut[i] = vbuf[i + 1];
      }

      tok = strtok(vbuf_cut, "\n");
      if (tok == NULL)
        tok = "";
      strncpy(dict[i].deutsch, tok, DICT_MAX_STRLEN);

      continue;
    } else {
      tok = strtok(vbuf, ";");
      if (tok == NULL)
        tok = "";

      strncpy(dict[i].english, tok, DICT_MAX_STRLEN);

      tok = strtok(NULL, "\n");
      if (tok == NULL)
        tok = "";

      strncpy(dict[i].deutsch, tok, DICT_MAX_STRLEN);
    }
  }
  fclose(fp);

  return dict;
}

int saveDictionaryToFile(char *dict_file, dictionary_t *dict, size_t dict_len) {
  FILE *fp = fopen(dict_file, "w");
  if (fp == NULL) {
    return 0;
  }

  for (int i = 0; i < dict_len; i++) {
    fprintf(fp, "%s;%s\n", dict[i].english, dict[i].deutsch);
  }

  fclose(fp);

  return 1;
}
