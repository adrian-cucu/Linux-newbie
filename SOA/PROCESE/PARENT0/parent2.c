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
	int status;

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
			printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			exit(EXIT_SUCCESS);
			// Codul 'nepotului'
		}
		waitpid(pID, &status, 0);
		exit(EXIT_SUCCESS);
		// Codul 'fiului'
	}

	// Codul procesului 'parinte'
	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());

	// Se asteapta fiul
	waitpid(pID, &status, 0);

	return 0;
}
