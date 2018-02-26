/**********************
    \File	    : dump.c
    \Author     : adrian 
    \Created on : Feb 26 2018 13:29 
 *********************/

/*
2. Sa se scrie in limbajul C  un program numit dump, 
care afiseaza in hexazecimal continutul unui fisier, octet cu octet. 
Afisarea continutului se face pe linei astfel:
fiecare  linie contine 16 octeti ai fisierului la inceputul fiecarei 
linii afisate se scrie (in hexa) adresa primului dintre cei 16 octeti 
de pe linie afisarea va contine o ultima linie suplimentara, in care se afla 
adresa ultimului octet al fisierului.
#) Numele fisierului al carui continut este afisat va fi dat de utilizator. 
#) Afisarea se va face astfel :
   $ ./dump f1.txt 
   0000000 61 62 63 64 20 61 &a 65 20 61 7a 65 72 09 72 61 
   0000020 0a 6c 6b 6a 64 20 64 66 66 09 6a 6b 0a 
   0000035
*/

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* Cati bytes sa contina buferul 
 in care citim cate un bloc din fisier
 il facem de 1KB 
*/
#define BUF_SIZE 1

/* Cati octeti contine fiecare linie */
#define DUMP_SIZE 16  

/* definim un tip de data octet => unsigned char adica un byte fara semn */
typedef unsigned char octet;  


/* Ne afiseaza 'len' octeti in format hexa */
void do_dump(octet *dump_buf, size_t len)
{
	while (len--) 
		printf(" %02x", *dump_buf++);
}


int main (int argc, char** argv)
{
	int i;
	int fd;
	int left;
	int addr;
	int dsize;
	size_t nbytes;
	octet buf[BUF_SIZE + 1];
	octet *out;

	if (argc < 2) {
		fprintf(stderr, "usage %s: <nume fisier>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	addr = 0x0;
	while ((nbytes = read(fd, buf, BUF_SIZE))) {
			
		left = nbytes;	

		/* out are adresa de inceput a buferului 
			in care am citit octetii din fisier
		*/
  		out = buf;

		/* Cat timp avem in bufer 
			mai multi octeti decat are o linie intreaga de dump
		*/
		while (left >= DUMP_SIZE) {
			printf("%08x", addr);	
			do_dump(out, DUMP_SIZE);
			putchar('\n');
			out += DUMP_SIZE;  /* Obtinem o noua adresa */
			addr += DUMP_SIZE; /* Incrementam adresa */	
			left -= DUMP_SIZE; /* Scadem din nr be octeti ramasi */
		}


		if (left) {	
			printf("%08x", addr);	
			do_dump(out, left);		
			putchar('\n');	
			addr += left;
		} 
	}	

	/* O linie suplimentara pentru adresa ultimului octet din fisier */
	printf("%08x\n", addr);	

	fflush(stdout);	
	close(fd);
	return 0;
}
