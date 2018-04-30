/*
5. Să se scrie un program in limbajul C ce conţine două procese (părinte + fiu).
Procesul părinte deschide un fişier text ce conţine cifre, litere mari, 
litere mici şi îl trimite printr-un pipe procesului fiu. 
Procesul fiu realizează o statistică cu numărul de caractere
primite din fiecare categorie (cifre, litere mari, litere mici) 
şi la final va afişa statistica respectivă. 
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>

#define NR_CARACTERE 5

pid_t bifurcare(void (*fun)(int*), int pip[2])
{
	pid_t pID;	
	switch( (pID = fork()) ) {
		case -1:
			perror("fork(): ");
			exit(EXIT_FAILURE);
		case 0:
			fun(pip);
		default:
			return pID;
	}
}

void nepot(int pip[2])
{
	int i;

	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
	close(pip[0]);

	for (i = 0; i < NR_CARACTERE; ++ i)
		write(pip[1], "A", 1);
	close(pip[1]);	
	exit(EXIT_SUCCESS);
}

void fiu(int pip[2])
{	
	int ncifre, nlitmari, nlitmici;
	int nbread, j;
	unsigned char buf[4096];
	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
	
	close(pip[1]);

	ncifre = nlitmari = nlitmici = 0;

	while ((nbread = read(pip[0], buf, 4096))) {
		for (j = 0; j < nbread; ++ j) {
			if (islower(buf[j])) ++nlitmici;
			else if (isupper(buf[j])) ++nlitmari;
			else if (isdigit(buf[j])) ++ncifre;
		}
	}
			

	printf("\r\nStats: \r\nCifre: %d\r\nLit. mari: %d\r\nLit. mici: %d\r\n",
		ncifre, nlitmari, nlitmici
	);

	close(pip[0]);
	exit(EXIT_SUCCESS);
}

int main (int argc, char** argv)
{
	pid_t pID;
	unsigned char buf[4096];
	char chr;
	int status;
	int p[2];
	int fd;
	int i;
	int nbread;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <nume fisier>\r\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("Eroare la descridere fisier");
		exit(EXIT_FAILURE);	
	}

	if (pipe(p) == -1) {
		perror("Nu am putut crea conductaaaaaaaa");
		exit(EXIT_FAILURE);
	}

	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
	pID = bifurcare(fiu, p);
	close(p[0]);


	while ((nbread = read(fd, buf, 4096))) {
		write(p[1], buf, nbread);
	}

	close(p[1]);
	waitpid(pID, &status, 0);
	return 0;
}
