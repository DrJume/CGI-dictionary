/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict_file.h"
#include "dictionary.h"
#include "html_snippets.h"

#define DICT_FILENAME "en-de_dict.csv"

char search[DICT_MAX_STRLEN + 1] = "";
char sortBy[11] = "";

void urldecode(char *url) {
  for (int i = 0; url[i] != '\0'; i++) {
    if (url[i] == '+')
      url[i] = ' ';
    if (url[i] == '%') {
      char escaped_char[3] = {url[i + 1], url[i + 2], 0}; // hex substring

      url[i] = (char)strtol(escaped_char, NULL, 16); // decode hex string to char

      memmove(url + i + 1, url + i + 3, strlen(url) - (i + 3)); // move rest over by 2
      memset(url + (strlen(url) - 2), '\0', 2);                 // and fill remains with \0
    }
  }
}

void GET(char *env[]) {
  html_Head();

  html_FormCreate(search, sortBy);

  puts("<br>");

  html_DataTable(DICT_FILENAME, search, sortBy);

  puts("<br>");

  // html_DebugInfo(env);

  html_Footer();
}

void POST_addEntry(char *english, char *deutsch) {
  // Read dictionary file
  size_t dict_len = 0;
  dictionary_t *dict = parseDictionaryFile(DICT_FILENAME, &dict_len);
  if (dict == NULL) {
    puts("");

    html_Head();
    html_FormCreate(search, sortBy);

    html_Error("Fehler beim Lesen der Wörterbuch-Datei!");
    puts("<br>");

    html_DataTable(DICT_FILENAME, search, sortBy);
    puts("<br>");

    // html_DebugInfo(env);
    html_Footer();

    return;
  }

  dictionary_t entry = {dict_len, "", ""};

  // Inject data into entry struct
  strncpy(entry.english, english, DICT_MAX_STRLEN);
  strncpy(entry.deutsch, deutsch, DICT_MAX_STRLEN);

  // Add entry to dictionary array
  dict = addDictionaryEntry(dict, dict_len, &entry);
  dict_len++;

  // Save new dict to file
  int success = saveDictionaryToFile(DICT_FILENAME, dict, dict_len);
  if (!success) {
    puts("");

    html_Head();
    html_FormCreate(search, sortBy);

    html_Error("Fehler beim Schreiben der Wörterbuch-Datei!");
    puts("<br>");

    html_DataTable(DICT_FILENAME, search, sortBy);
    puts("<br>");

    // html_DebugInfo(env);
    html_Footer();
    return;
  }

  // When successfull, reload page with GET request
  puts("Status: 303 See Other");
  printf("Location: %s\n", getenv("REQUEST_URI"));
  puts("");
}

void POST_deleteEntry(int index) {
  // Read dictionary file
  size_t dict_len = 0;
  dictionary_t *dict = parseDictionaryFile(DICT_FILENAME, &dict_len);

  // Check if delete index invalid
  if (index < 0 || index > dict_len) {
    puts("Status: 400 Bad Request");
    puts("");
    puts("Ungültiger Wert für POST-Argument 'index='");
    return;
  }

  dict = deleteDictionaryEntry(dict, dict_len, index);
  dict_len--;

  int success = saveDictionaryToFile(DICT_FILENAME, dict, dict_len);
  if (!success) {
    puts("");

    html_Head();
    html_FormCreate(search, sortBy);

    html_Error("Fehler beim Schreiben der Wörterbuch-Datei!");
    puts("<br>");

    html_DataTable(DICT_FILENAME, search, sortBy);
    puts("<br>");

    // html_DebugInfo(env);
    html_Footer();
    return;
  }

  // When successfull, reload page with GET request
  puts("Status: 303 See Other");
  printf("Location: %s\n", getenv("REQUEST_URI"));
  puts("");
}

void POST_Debug() {
  puts("");

  puts("<pre>");
  puts("POST-BODY:\n");

  char keyval[80];

  while (fgets(keyval, 80, stdin)) {
    printf(keyval);
  }

  puts("</pre>");
  return;
}

void POST(char *env[]) {
  // POST_Debug();
  // return;

  // Read POST Body
  char keyval[80];

  // Read needed variables (from key-value)
  char action[11] = "";

  char delete_index[11] = "";
  char add_english[DICT_MAX_STRLEN + 1] = "";
  char add_deutsch[DICT_MAX_STRLEN + 1] = "";

  while (fgets(keyval, 80, stdin)) {
    if (strstr(keyval, "action=") != NULL) {
      char *tok = strtok(keyval, "=");
      tok = strtok(NULL, "\n");
      tok[strlen(tok) - 1] = 0; // remove last ctrl character
      strncpy(action, tok, 10);
    }
    if (strstr(keyval, "index=") != NULL) {
      char *tok = strtok(keyval, "=");
      tok = strtok(NULL, "\n");
      tok[strlen(tok) - 1] = 0; // remove last ctrl character
      strncpy(delete_index, tok, 10);
    }
    if (strstr(keyval, "english=") != NULL) {
      char *tok = strtok(keyval, "=");
      tok = strtok(NULL, "\n");
      tok[strlen(tok) - 1] = 0; // remove last ctrl character
      strncpy(add_english, tok, DICT_MAX_STRLEN);
    }
    if (strstr(keyval, "deutsch=") != NULL) {
      char *tok = strtok(keyval, "=");
      tok = strtok(NULL, "\n");
      tok[strlen(tok) - 1] = 0; // remove last ctrl character
      strncpy(add_deutsch, tok, DICT_MAX_STRLEN);
    }
  }

  // When no POST action present, reload page with GET request
  if (strcmp(action, "") == 0) {
    puts("Status: 400 Bad Request");
    puts("");
    puts("Fehlendes POST-Argument 'action='");
    return;
  }

  if (strstr(action, "add") != NULL) { // Add dictionary entry
    // Check if input is empty
    if (strlen(add_english) == 0 || strlen(add_english) == 0) {
      puts("");

      html_Head();
      html_FormCreate(search, sortBy);

      html_Error("Eingabe darf nicht leer sein!");
      puts("<br>");

      html_DataTable(DICT_FILENAME, search, sortBy);
      puts("<br>");

      // html_DebugInfo(env);
      html_Footer();

      return;
    }

    POST_addEntry(add_english, add_deutsch);

  } else if (strstr(action, "delete") != NULL) { // Delete dictionary entry
    // When no delete index present, reload page with GET request
    if (strlen(delete_index) == 0) {
      puts("Status: 400 Bad Request");
      puts("");
      puts("Fehlendes POST-Argument 'index='");
      return;
    }

    POST_deleteEntry(atoi(delete_index));

  } else { // Handle unknown POST action: reload page with GET request
    puts("Status: 400 Bad Request");
    puts("");
    puts("Ungültiger Wert für POST-Argument 'action='");
    return;
  }
}

int main(int argc, char *argv[], char *env[]) {
  puts("Content-Type: text/html; charset=utf-8");

  // Handle Request query string
  char queryString[strlen(getenv("QUERY_STRING")) + 1];
  strcpy(queryString, getenv("QUERY_STRING"));

  // Count key-value pairs in query string
  int keyvalCount = 0;
  for (char *keyval_tok = strtok(queryString, "&"); keyval_tok != NULL;
       keyval_tok = strtok(NULL, "&")) {
    keyvalCount++;
  }
  strcpy(queryString, getenv("QUERY_STRING"));

  // Put key-value pair pointers into array
  char *keyvals[keyvalCount];
  int i = 0;
  for (char *keyval_tok = strtok(queryString, "&"); keyval_tok != NULL;
       keyval_tok = strtok(NULL, "&")) {

    keyvals[i] = calloc(strlen(keyval_tok) + 1, sizeof(char));
    strcpy(keyvals[i], keyval_tok);
    i++;
  }

  for (int i = 0; i < keyvalCount; i++) {
    char *keyval = keyvals[i];

    if (strstr(keyval, "search=")) {
      char *tok = strtok(keyval, "=");

      tok = strtok(NULL, "&");
      if (tok != NULL) {
        urldecode(tok);
        strncpy(search, tok, DICT_MAX_STRLEN);
      } else {
        strcpy(search, "");
      }

      continue;
    }

    if (strstr(keyval, "sortBy=")) {
      char *tok = strtok(keyval, "=");

      tok = strtok(NULL, "&");
      if (tok != NULL) {
        strncpy(sortBy, tok, 10);
      } else {
        strcpy(sortBy, "");
      }

      continue;
    }
  }

  if (strcmp(getenv("REQUEST_METHOD"), "GET") == 0) {
    puts("");
    GET(env);
  } else if (strcmp(getenv("REQUEST_METHOD"), "POST") == 0) {
    POST(env);
  }

  return 0;
}
