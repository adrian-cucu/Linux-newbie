/*
3. Sa se scrie in limbajul C  un program numit wordcount, 
care afiseaza informatii despre continutul 
unui fisier text : 
numarul de  caractere , 
numarul de cuvinte, 
numarul de linii din fisier.
Numele fisierului ale carui caractere, cuvinte, 
linii sunt numarate va fi dat de utilizator.
Precizari : 
(1) liniile vide sunt numarate ; 
(2) un cuvant este un sir de caractere care contin caracater non-blank (nu contine spatii, tab-uri, enter, aceste caractere sunt considerate separatori de cuvant). 
Afisarea de va face astfel:

$ ./worcount f1.txt 
 2	7 	29
*/

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#define BUF_SIZE 512

int main(int argc, char** argv) 
{	
	int fd;
	char *fis;
	char buf[BUF_SIZE + 1];
	int cuv_nou = 1;
	unsigned char *chr;
	size_t nbytes;	
	size_t caractere = 0;
	size_t cuvinte = 0;
	size_t linii = 0;

	if (argc < 2) {
		fprintf	(stderr, "usage %s: <nume fisier>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fis = argv[1];
	if ((fd = open(fis, O_RDONLY)) == -1) {
		perror("open");		
		exit(EXIT_FAILURE);
	}
	
	while ((nbytes = read(fd, buf, BUF_SIZE))) {
		buf[nbytes] = '\0';
		caractere += nbytes;
		chr = buf;
		while (*chr) {
			if (isspace(*chr)) {
				if (*chr == '\n')++linii;	
				cuv_nou = 1;
			} else if (cuv_nou) {
				++cuvinte;
				cuv_nou = 0;
			}
			++chr;
		}
	}	

	printf ("%7lu %7lu %7lu\n", linii, cuvinte, caractere);

	
	return 0;
}
