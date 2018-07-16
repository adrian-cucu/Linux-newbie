/*
4. Sa se scrie in limbajul C  un program care creeaza 
filiatia de procese ilustrata in figura de mai jos. 
Fiecare proces va afisa un text cu numele sau 
(parinte, fiu, nepot, etc),  precum si pid-ul propriu si cel al parintelui. 
Sa se trasforme procesul nepot1 in ZOMBI iar nepot2 in ORFAN.
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


#define error(msg) \
	do { if(msg) perror(msg); exit(EXIT_FAILURE); } while(0)

void afisarePID(const char *nume)
{
	if (nume) 
		printf("%s PID propriu:={\033[32m%d\033[00m}"
				  "PID parinte:={\033[32m%d\033[00m}\r\n", 
			nume, (int)getpid(), (int)getppid());
	else
		printf("PID propriu:={\033[32m%d\033[00m}"
			   "PID parinte:={\033[32m%d\033[00m}\r\n", 
			(int)getpid(), (int)getppid());	
}

int main (int argc, char** argv)
{
	pid_t pID1, pID2, pID3;
	int cnt;
	int status;
	int kids;
	struct timespec tm = {.tv_sec = 5, .tv_nsec = 0};
	/*
		struct timespec {
               time_t tv_sec;        // seconds 
               long   tv_nsec;       // nanoseconds 
           };
	*/
	afisarePID("- PARINTE");

	// Crearea fiu 1
	pID1 = fork();
	if (pID1 == -1) error("esec la crearea fiului");

	if (pID1 == 0) {
		// Codul fiului 1
		afisarePID("- FIU");	
		pID2 = fork();
		if (pID2 == -1) error("esec la crearea nepotului 1");	
		if (pID2 ==  0) {
			//Codul nepotului 1
			afisarePID("--- NEPOT 1");
			exit(EXIT_SUCCESS);
		}
		pID3 = fork();
		if (pID3 == -1) error("esec la crearea nepotului 2");	
		if (pID3 ==  0) {
			//Codul nepotului 2
			while (1) {
				afisarePID("--- NEPOT 2");
				nanosleep(&tm, NULL);	
			}
				
			exit(EXIT_SUCCESS);
		}
		nanosleep(&tm, NULL);	

		exit(EXIT_SUCCESS);
	}

	if (waitpid(pID1, &status, 0) == pID1) {
		printf("Procesul %d a fost inchis cod retur: %d\r\n", 
			(int)pID2, WEXITSTATUS(status)
		);
	}

	return 0;
}
