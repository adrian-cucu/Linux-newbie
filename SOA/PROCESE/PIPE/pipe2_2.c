/*
2. Sa se scrie in limbajul C trei programe numite 
respectiv parent1 , parent2 si parent3 care creeaza 
filiatia de procese ilustrata in cele 3 figuri de mai jos.
In cele 3 programe, fiecare proces sa afiseze un text cu 
numele sau (parinte, fiu, nepot, etc), precum si pid-ul 
propriu si cel al parintelui. 

parent2
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
	int status;
	pid_t pID;

	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
	pID = bifurcare(nepot, pip);
	close(pip[0]);
	close(pip[1]);
	waitpid(pID, &status, 0);
	exit(EXIT_SUCCESS);
}

int main (int argc, char** argv)
{
	pid_t pID;
	char chr;
	int status;
	int p[2];
	int i;

	if (pipe(p) == -1) {
		perror("Nu am putut crea conductaaaaaaaa");
		exit(EXIT_FAILURE);
	}

	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
	pID = bifurcare(fiu, p);

	for (i = 0; i < NR_CARACTERE; ++ i) {
		read(p[0], &chr, 1);
		write(1, &chr, 1);
	}
	write(1, "\n", 1);

	close(p[0]);
	close(p[1]);
	waitpid(pID, &status, 0);
	return 0;
}
