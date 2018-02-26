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

#define BUF_SIZE 1
#define MAX_LINE 255  /*Lungimea maxima de caractere pe o linie a fisierului*/


int main(int argc, char** argv) 
{	
	int fd;
	char cuv[50];
	char buf[BUF_SIZE + 1];
	char line[MAX_LINE + 1];
	char fisier[256];
	char *ptr, *p1, *p2, *c;
	size_t nbytes;
	ssize_t cat;

	printf("Introduceti numele fisierului : "); 
	scanf("%s", fisier);

	if ((fd = open(fisier, O_RDONLY)) == -1) {
		perror("open");		
		exit(EXIT_FAILURE);
	}

	printf("Fisierul este:\n");	
	/*Afisam continutul fisierului*/	
	while ((nbytes = read(fd, buf, BUF_SIZE))) {
		write(STDOUT_FILENO, buf, nbytes);
	}

	printf("Intoduceti sirul de cautat : ");
	scanf("%s", cuv);

	/*Ne intoarcem la inceputul fisierului*/
	lseek(fd, 0, SEEK_SET);	
	
	printf("Liniile care contin sirul \"%s\":\n", cuv);
	/*Dinou citim blocuri a cate BUF_SIZE octeti din fisier
	  dar exista riscul ca o linie sa fie incadrata
	  intre 2 ciitiri de blocuri succesive
	  deci aceasta problema trebuie tratata
	*/
	ptr = line;
	while ((nbytes = read(fd, buf, BUF_SIZE))) {

		buf[nbytes] = '\0';

		for (p1 = buf, p2 = p1; *p2; p1 = p2) {
			while (*p2 && *p2 != '\n') {
				++p2;
			}		

			if (*p2 == '\n') {
				*p2 = '\0';
				strcpy(ptr, p1);	

				if ((c = strstr(line, cuv))) {	
					write(STDOUT_FILENO, line, c - line);	
					printf("\033[33m%s\033[00m", cuv);
					printf("%s\n", line + (c - line) + strlen(cuv));
				}
				ptr = line;
				++p2;

			} else {
				strcpy(ptr, p1);
				ptr += (p2 - p1);
			}	
		}
	}
	
	return 0;
}
