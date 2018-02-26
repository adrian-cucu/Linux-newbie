/*
  \Autor: acu
  \Fisier: prog.c
  \Compilat cu: gcc prog.c -o prog
  \Rularea executabil: ./prog prog.c 0 1300
  \Cerinta:
Sa se scrie in limbajul C  un program care citeste 
si afiseaza un grup de octeti dintr-un fisier, 
cuprinsi intre pozitiile "m" si "n". 
Cele doua valori m si n sunt furnizate de utilizator.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define SUCCESS 0
#define EROARE  1


int main (int argc, char** argv)
{
	char *nume_fisier;
	char buf[BUF_SIZE + 1];
	int fd;
	int m, n;
	int nbytes, lbytes;
	off_t marime_fisier;
	int status = SUCCESS;

	if (argc < 4) {
		fprintf(stderr, "folosire %s: <nume fisier> <m> <n>\n"
					"0 <= m < n < lungimea fisierului in octeti\n", argv[0]);
		exit(EXIT_FAILURE);
	}	

	nume_fisier = argv[1];	
	m = atoi(argv[2]); // din char* convertim in int
	n = atoi(argv[3]);

	if (m >= n || m < 0 || n < 0) {
		fprintf(stderr, "eroare parametri: m = %d, n = %d\n", m, n);
		return EROARE;
	}	

	fd = open(nume_fisier, O_RDONLY);
	if (fd == -1) {		
		perror("open");
		return EROARE;
	}

	marime_fisier = lseek(fd, 0, SEEK_END);
	if (marime_fisier == (off_t)-1) {
		perror("lseek");
		status = EROARE;
		goto sfarsit;
	}
	
	if (marime_fisier < m) {
		fprintf(stderr, "Fisierul are mai putin de %d octeti!\n", m);	
		status = EROARE;
		goto sfarsit;
	}

	if (marime_fisier < n) {
		fprintf(stderr, "Fisierul are mai putin de %d octeti!\n", n);
		status = EROARE;
		goto sfarsit;
	}

	if(lseek(fd, m, SEEK_SET) == (off_t)-1) {
		perror("lseek");	
		status = EROARE;
		goto sfarsit;

	}
	
	lbytes = n - m; // cati octeti mai avem de citit
	while (lbytes > 0) { 
		
		if ((nbytes = read(fd, buf, BUF_SIZE)) == 0) {
				fprintf(stderr, "A aparut o eroare la citire\n");
				status = EROARE;
				goto sfarsit;
		}

		if (lbytes - nbytes >= 0) {
			write(STDOUT_FILENO, buf, nbytes);
		} else {
			write(STDOUT_FILENO, buf, lbytes);
		}
		lbytes -= nbytes;
	} 
	putchar('\n');
	fflush(stdout);
	sfarsit:
		close(fd);
		return status;
}
