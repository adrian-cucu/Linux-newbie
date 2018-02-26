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

#define BUF_SIZE 512
#define DUMP_SIZE 16
typedef unsigned char octet;  

void do_dump(void *dump_buf, size_t len)
{
	octet *dmp = (octet *)dump_buf;

	while (len--) 
		printf(" %02x", *dmp++);
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
  		out = &buf[0];

		while (left >= DUMP_SIZE) {
			printf("%08x", addr);	
			do_dump(out, DUMP_SIZE);
			putchar('\n');
			out += DUMP_SIZE;
			addr += DUMP_SIZE;	
			left -= DUMP_SIZE;
		}

		if (left) {
			printf("%08x", addr);	
			do_dump(out, left);		
			putchar('\n');	
			addr += left;
		} 
	}	

	printf("%08x\n", addr);	

	fflush(stdout);	
	close(fd);
	return 0;
}
