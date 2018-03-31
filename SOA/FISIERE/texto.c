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
/*Lungimea maxima de caractere pe o linie a fisierului 
nu trebuie sa depaseasca lungimea bufereului */

#define ROSU 		"\033[31m"
#define VERDE 		"\033[32m"
#define ALBASTRU 	"\033[34m"
#define INCOLOR 	"\033[00m"


int main(int argc, char** argv) 
{	
	int fd;
	char *fisier, *cuvant, *inceput_linie, *sfarsit_linie, *c;
	char buf[BUF_SIZE + 1], *buf_start;
	int buf_size;
	int nbytes;
	int lungime_linie;

	if (argc < 3) {
		fprintf	(stderr, "usage %s: <nume fisier> <cuvant cautat>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fisier = argv[1];
	cuvant = argv[2];

	if ((fd = open(fisier, O_RDONLY)) == -1) {
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
		fprintf(stderr, "0 octeti cititi din fisierul: %s \n", fisier);
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
	buf_size = BUF_SIZE; 
	buf_start = buf;

	while ((nbytes = read(fd, buf_start, buf_size)) > 0) {

		buf_start[nbytes] = '\0';
		inceput_linie = &buf[0];	

		while ( (sfarsit_linie = strchr(inceput_linie, '\n')) ) {

			*sfarsit_linie = '\0';
	
			lungime_linie = sfarsit_linie - inceput_linie;
						
			/*Cautam in linia curenta cuvantul dat*/
			c = strstr(inceput_linie, cuvant);	
			

			if (c) {
				write(STDOUT_FILENO, inceput_linie, c - inceput_linie);
				write(STDOUT_FILENO, VERDE, sizeof(ROSU));		
				write(STDOUT_FILENO, c, strlen(cuvant));
				write(STDOUT_FILENO, INCOLOR, sizeof(INCOLOR));		
				write(STDOUT_FILENO, c + strlen(cuvant), 
					lungime_linie - ((c + strlen(cuvant)) - inceput_linie));
				write(STDOUT_FILENO, "\n", 1);
			}

			inceput_linie = ++sfarsit_linie;
		}		
	
		if (*inceput_linie) {
			memcpy(buf, inceput_linie, lungime_linie);
			buf_start = buf + lungime_linie;
			buf_size = BUF_SIZE - lungime_linie;
		}	
	}
	putchar('\n');
	
	return 0;
}
