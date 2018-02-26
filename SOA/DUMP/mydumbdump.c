/*
2. Sa se scrie in limbajul C  un program numit dump , care afiseaza in hexazecimal continutul unui fisier, octet cu octet. Afisarea continutului se face pe linei astfel :
fiecare  linie contine 16 octeti ai fisierului 
la inceputul fiecarei linii afisate se scrie (in hexa) adresa primului dintre cei 16 octeti de pe linie 
afisarea va contine o ultima linie suplimentara, in care se afla adresa ultimului octet al fisierului.
#Numele fisierului al carui continut este afisat va fi dat de utilizator. 
Afisarea se va face astfel :
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

int main (int argc, char** argv)
{
	int i;
	int fd;
	char *file_name;
	int left;
	int addr;
	int dsize;
	size_t nbytes;
	octet buf[BUF_SIZE + 1];
	octet dump[DUMP_SIZE + 1];
	octet *ptr, *out, *p1;

	if (argc < 2) {
		fprintf(stderr, "usage %s: <nume fisier>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	file_name = argv[1];

	if ((fd = open(file_name, O_RDONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	addr = 0x0;
	dsize = DUMP_SIZE;
	p1 = dump;
	while ((nbytes = read(fd, buf, BUF_SIZE))) {
		
		/* Copiem octetii din bufer in dump */
		ptr = buf;
		left = nbytes;	
		do
		  {
			memcpy(p1, ptr, dsize);
				
			printf("%08x ", addr);	

			for (out = dump, i = 0; i < DUMP_SIZE; ++ i) {
				printf("%02x ", *(out + i));
			}				
			putchar(10);

			addr += 0x10;	
			left -= DUMP_SIZE;
			ptr += DUMP_SIZE;
	 	} while (left > DUMP_SIZE);		

		if (left) {
			memcpy(p1, ptr, left);
			p1 += left;
			dsize = DUMP_SIZE - left;
			addr += left;
		} else {
			p1 = dump;
			dsize = DUMP_SIZE;
		}

	}	

	if (left) {
		printf("%08x ", addr - left);	
		for (out = dump, i = 0; i < left; ++ i) {
				printf("%02x ", *(out + i));
		}				
		putchar(10);
	}
	printf("%08x\n", addr);	

	fflush(stdout);	
	close(fd);
	return 0;
}
