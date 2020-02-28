/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <string.h>

#include "dictionary.h"

void html_Head();

void html_Footer();

void html_FormCreate(char *search, char *sortBy);

void html_DataTable(char *dict_filename, char *search, char *sortBy);

void html_DebugInfo(char* env[]);

void html_Error(char* errStr);