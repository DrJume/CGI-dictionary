/*
  Autor: Julian Meinking
  Datum: 28.02.2020
*/

#include <stdio.h>
#include <string.h>

#include "dict_file.h"
#include "dictionary.h"

void html_Head() {
  puts("<!doctype html>");

  puts("<html>");
  puts("<head>");
  puts("  <title>Englisch-Deutsch Wörterbuch</title>");
  puts("  <meta charset=\"utf-8\"/>");
  puts("    <link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\">");
  puts("</head>");

  puts("<body>");

  puts("<h1>Englisch-Deutsch Wörterbuch</h1>");
  puts("<br>");
}

void html_Footer() {
  puts("</body>");
  puts("</html>");
}

void html_FormCreate(char *search, char *sortBy) {
  // Form for adding entry
  puts("<div class=\"form-control\">");

  puts("<form action=\"\" method=\"POST\" enctype=\"text/plain\">");
  puts("<table>");

  puts("<tr><td>");
  puts("  <label for=\"english\">English:</label>");
  puts("</td><td>");
  puts("   <input type=\"text\" id=\"english\" name=\"english\" "
       "maxlength=\"DICT_MAX_STRLEN\" size=\"30\" required>");
  puts("</td><td></td><td rowspan=\"2\">");
  puts("  <input type=\"submit\" value=\"Eintrag hinzufügen\">");
  puts("</td></tr>");

  puts("<tr><td>");
  puts("  <label for=\"deutsch\">Deutsch:</label>");
  puts("</td><td>");
  puts("   <input type=\"text\" id=\"deutsch\" name=\"deutsch\" "
       "maxlength=\"DICT_MAX_STRLEN\" size=\"30\" required>");
  puts("  <input type=\"text\" name=\"action\" value=\"add\" hidden>");
  puts("</td></tr>");

  puts("</table>");
  puts("</form>");

  puts("<br>");
  puts("<hr>");
  puts("<br>");

  // Search and sort form
  puts("<form action=\"\" method=\"GET\" enctype=\"text/plain\">");
  puts("<table>");

  puts("<tr><td>");
  puts("  <label for=\"search\">Suche:</label>");
  puts("</td><td>");
  printf("<input type=\"text\" id=\"search\" name=\"search\" maxlength=\"DICT_MAX_STRLEN\" "
         "size=\"30\" "
         "value=\"%s\">\n",
         search);
  puts("</td><td>");
  puts("<input type=\"submit\" style=\"width: 100%;\" value=\"Suchen\">");
  puts("</td><td>");
  printf("<button onclick=\"document.querySelector('#search').value = '';\">Zurücksetzen</button>");
  puts("</td></tr>");

  puts("<tr><td></td></tr><tr><td>");

  puts("  <label for=\"sort_select\">Sortieren nach:</label>");

  puts("</td><td>");

  puts("<select id=\"sort_select\" name=\"sortBy\">");
  printf("  <option value=\"\" %s>-</option>\n", strcmp(sortBy, "") == 0 ? "selected" : "");
  printf("  <option value=\"english\" %s>Englisch</option>\n",
         strcmp(sortBy, "english") == 0 ? "selected" : "");
  printf("  <option value=\"deutsch\" %s>Deutsch</option>\n",
         strcmp(sortBy, "deutsch") == 0 ? "selected" : "");
  puts("</select>");

  // printf("<input type=\"radio\" id=\"sort_english\" name=\"sortBy\" value\"english\"
  // checked>\n"); puts("<label for=\"sort_english\" style=\"margin-right:
  // 10px;\">Englisch</label>");

  // printf("<input type=\"radio\" id=\"sort_deutsch\" name=\"sortBy\" value\"deutsch\">\n");
  // puts("<label for=\"sort_deutsch\">Deutsch</label>");

  puts("</td><td>");
  puts("  <input type=\"submit\" value=\"Sortieren\">");
  puts("</td></tr>");

  puts("</table>");
  puts("</form>");
  puts("</div>");

  puts("<br>");
}

void html_DataTable(char *dict_filename, char *search, char *sortBy) {
  size_t dict_len = 0;
  dictionary_t *dict = parseDictionaryFile(dict_filename, &dict_len);
  if (dict == NULL) {
    printf("<h3 style='color: red;'>"
           "Wörterbuch-Datei %s konnte nicht geöffnet werden!</h3>\n",
           dict_filename);
    return;
  }

  puts("<table class=\"data-table\">");
  puts("<thead><tr>");
  puts("<th>Englisch</th>");
  puts("<th>Deutsch</th>");
  puts("<th width=\"50px\">Aktion</th>");
  puts("</tr></thead>");

  puts("<tbody>");
  int entries = 0;
  for (int i = 0; i < dict_len; i++) {
    if (strcmp(sortBy, "") != 0) { // sorting is active
      sortDictionary(dict, dict_len, sortBy);
    }
    if (strcmp(search, "") != 0) { // search is active
      // if search string not found in either column
      if (strcasestr(dict[i].english, search) == NULL &&
          strcasestr(dict[i].deutsch, search) == NULL) {
        continue;
      }
    }

    entries++;

    puts("<tr>");
    printf("<td>%s</td>\n", dict[i].english);
    printf("<td>%s</td>\n", dict[i].deutsch);

    printf("<td><form action=\"\" method=\"POST\" enctype=\"text/plain\">");
    printf("<input type=\"text\" name=\"index\" value=\"%d\" hidden>", dict[i].index);
    printf("<input type=\"text\" name=\"action\" value=\"delete\" hidden>");
    printf("<input type=\"submit\" value=\"Löschen\">");
    printf("</form></td>");
    puts("</tr>");
  }
  puts("</tbody>");

  puts("</table>");

  printf("<p>%d Einträge gefunden.</p>\n", entries);
}

void html_DebugInfo(char *env[]) {
  puts("<hr><pre>");

  char **pEnv = env;

  while (*pEnv) {
    puts(*pEnv);
    pEnv++;
  }

  puts("</pre>");
}

void html_Error(char *errStr) { printf("<span class=\"error\">%s</span>\n", errStr); }
