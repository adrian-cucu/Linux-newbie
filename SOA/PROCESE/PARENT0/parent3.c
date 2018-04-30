/*
2. Sa se scrie in limbajul C trei programe numite 
respectiv parent1 , parent2 si parent3 care creeaza 
filiatia de procese ilustrata in cele 3 figuri de mai jos.
In cele 3 programe, fiecare proces sa afiseze un text cu 
numele sau (parinte, fiu, nepot, etc), precum si pid-ul 
propriu si cel al parintelui. 

parent3
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
	pid_t pID1, pID2;
	int status;

	// Crearea fiu 1
	pID1 = fork();
	if (pID1 == -1) {
		perror("esec la crearea fiului 1");
		exit(EXIT_FAILURE);
	}
	if (pID1 == 0) {
		// Codul 'fiului 1'
		printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			
		// Creare nepot 2
		pID2 = fork();
		if (pID2 == -1) {
			perror("esec la crearea nepotului 1");
			exit(EXIT_FAILURE);
		}
		if (pID2 == 0) {
			// Codul 'nepotului 1'
			printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			exit(EXIT_SUCCESS);
			// Codul 'nepotului 1'
		}
		waitpid(pID2, &status, 0);
		exit(EXIT_SUCCESS);
		// Codul 'fiului 1'
	}

	// Crearea fiu 2
	pID2 = fork();
	if (pID2 == -1) {
		perror("esec la crearea fiului 1");
		exit(EXIT_FAILURE);
	}
	if (pID2 == 0) {
		// Codul 'fiului 2'
		printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			
		// Creare nepot 2
		pID2 = fork();
		if (pID2 == -1) {
			perror("esec la crearea nepotului 1");
			exit(EXIT_FAILURE);
		}
		if (pID2 == 0) {
			// Codul 'nepotului 2'
			printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());	
			exit(EXIT_SUCCESS);
			// Codul 'nepotului 2'
		}
		waitpid(pID2, &status, 0);
		exit(EXIT_SUCCESS);
		// Codul 'fiului 2'
	}


	// Codul procesului 'parinte'
	printf("PID={%d}\tParent PID={%d}\r\n", getpid(), getppid());

	// Se asteapta fiul 1
	waitpid(pID1, &status, 0);

	// Se asteapta fiul 2
	waitpid(pID2, &status, 0);


	return 0;
}
