/**********************
    \File	    : texto.c
    \Author     : adrian 
    \Created on : Feb 25 2018 21:35 
 *********************/
/*
Sa se scrie in limbajul C  un program numit texto ,  care :
afiseaza continutul unui fisier text
cauta un sir de caractere in cadrul acelui unui fisier text. 
Sirul de caractere si numele fisierului sunt date de utilizator. 
Afisarea se va face astfel :

Introduceti numele fisierului : f1.txt  
Fisierul este:
Capra sare piatra
Piatra crapa in patru
Crapa-i-ar capul caprei
Cum a crapat piatra in patru
Intoduceti sirul de cautat : cap
Liniile care contin sirul "cap" :
Capra sare piatra
Crapa-i-ar capul caprei
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
#define MAX_LINE 255  /*Lungimea maxima de caractere pe o linie a fisierului*/


int main(int argc, char** argv) 
{	
	int fd;
	char *fis;
	char *cuv;
	char buf[BUF_SIZE + 1];
	char line_buf[MAX_LINE + 1];
	char *lbuf_pos;
	char *p1;
	char *p2;
	char *c;
	int nbytes;

	if (argc < 3) {
		fprintf	(stderr, "usage %s: <nume fisier> <cuvant cautat>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fis = argv[1];
	cuv = argv[2];

	if ((fd = open(fis, O_RDONLY)) == -1) {
		perror("open");		
		exit(EXIT_FAILURE);
	}
	
	nbytes = read(fd, buf, BUF_SIZE);

	/* Eroare la citire */
	if (nbytes ==  -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	/* Fisierul este gol */
	if (nbytes == 0) {
		fprintf(stderr, "0 octeti cititi din fisierul: %s \n", fis);
		exit(EXIT_FAILURE);
	}	

	/* Afisam continutul fisierului */
	do 
	  {
		write(STDOUT_FILENO, buf, nbytes);

	} while ((nbytes = read(fd, buf, BUF_SIZE)));


	/*Ne intoarcem la inceputul fisierului*/
	lseek(fd, 0, SEEK_SET);	

	printf("\n=========================\n");

	/*Dinou citim blocuri a cate BUF_SIZE octeti din fisier
	  dar exista riscul ca o linie sa fie incadrata
	  intre 2 ciitiri de blocuri succesive
	  deci aceasta problema trebuie tratata
	*/
	lbuf_pos = line_buf;
	while ((nbytes = read(fd, buf, BUF_SIZE)) > 0) {

		buf[nbytes] = '\0';

		for (p1 = buf, p2 = p1; *p2; p1 = p2) {
			while (*p2 && *p2 != '\n') {
				++p2;
			}		

			if (*p2 == '\n') {
				*p2 = '\0';
				strcpy(lbuf_pos, p1);	

				if ((c = strstr(line_buf, cuv))) {	
					write(STDOUT_FILENO, line_buf, c - line_buf);	
					printf("\033[33m%s\033[00m", cuv);
					printf("%s\n", line_buf + (c - line_buf) + strlen(cuv));
				}
				lbuf_pos = line_buf;
				++p2;

			} else {
				strcpy(lbuf_pos, p1);
				lbuf_pos += (p2 - p1);
			}	
		}
	}
	
	return 0;
}
