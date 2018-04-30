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

void handler(int sig, siginfo_t *sinfo, void *ptr)
{	
	printf("si_code: %d\r\n", sinfo->si_code);
	printf("si_pid: %d\r\n", sinfo->si_pid);
	printf("si_uid: %d\r\n", sinfo->si_uid);

	printf("----------------------------------\r\n");
}


int main (int argc, char** argv)
{
	pid_t pID;
	char buffer[512];
	int status;
	int p[2];

	if (pipe(p) == -1) {
		perror("Nu am putut crea conductaaaaaaaa");
		exit(EXIT_FAILURE);
	}

	// Crearea fiu
	pID = fork();
	if (pID == -1) {
		perror("esec la crearea fiului");
		exit(EXIT_FAILURE);
	}
	if (pID == 0) {
		// Codul 'fiului'
		printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			
		// Creare nepot
		pID = fork();
		if (pID == -1) {
			perror("esec la crearea nepotilui");
			exit(EXIT_FAILURE);
		}
		if (pID == 0) {
			// Codul 'nepotului'
			close(p[0]);

			printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	

			memset(buffer, 'A', 512);
			write(p[1], buffer, 512);
			write(1, "scris\n", 6);
			close(p[1]);	
			exit(EXIT_SUCCESS);
			// Codul 'nepotului'
		}

		close(p[0]);
		close(p[1]);
		waitpid(pID, &status, 0);
		exit(EXIT_SUCCESS);
		// Codul 'fiului'
	}

	// Codul procesului 'parinte'
	close(p[1]);
	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());

	read(p[0], buffer, 512);

	write(1, "citit\n", 6);
	
	write(1, buffer, 512);
	write(1, "\n", 1);
	// Se asteapta fiul
	waitpid(pID, &status, 0);

	close(p[0]);
	return 0;
}
