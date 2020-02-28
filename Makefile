#	Autor: Julian Meinking
#	Datum: 28.02.2020

CFLAGS=-c -Wall

output: main.o dict_file.o dictionary.o html_snippets.o
	gcc main.o dict_file.o dictionary.o html_snippets.o -o index.cgi

main.o: main.c
	gcc $(CFLAGS) main.c

dict_file.o: dict_file.c
	gcc $(CFLAGS) dict_file.c

dictionary.o: dictionary.c
	gcc $(CFLAGS) dictionary.c

html_snippets.o: html_snippets.c
	gcc $(CFLAGS) -D_GNU_SOURCE html_snippets.c

clean:
	rm *.o index.cgi

# install:
	# cp index.cgi en-de_dict.csv styles.css /var/www/html/cgi-bin/