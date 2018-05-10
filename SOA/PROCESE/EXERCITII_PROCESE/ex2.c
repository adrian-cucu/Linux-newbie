/*
2. Sa se scrie in limbajul C  un program care creeaza 
filiatia de procese ilustrata in figura de mai jos. 
Fiecare proces va afisa un text cu numele sau (parinte, fiu, nepot, etc),  
precum si pid-ul propriu si cel al parintelui. 
Sa se transforme in ZOMBI procesul nepot.
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
/*
void handler(int sig, siginfo_t *sinfo, void *ptr)
{	
	printf("si_code: %d\r\n", sinfo->si_code);
	printf("si_pid: %d\r\n", sinfo->si_pid);
	printf("si_uid: %d\r\n", sinfo->si_uid);

	printf("----------------------------------\r\n");
}
*/

void afisarePID(const char *nume)
{
	if (nume) 
		printf("- %s PID:={%d} PID parinte:={%d}\r\n", 
			nume, (int)getpid(), (int)getppid()
		);
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
	afisarePID("parinte");

	// Crearea fiu 1
	pID1 = fork();
	if (pID1 == -1) {
		perror("esec la crearea fiului 1");
		exit(EXIT_FAILURE);
	}
	if (pID1 == 0) {
		// Codul fiului 1
		afisarePID("FIU 1");	
		pID3 = fork();
		if (pID3 == -1) {
			perror("esec la crearea nepotului");
			exit(EXIT_FAILURE);
		}
	
		if (pID3 == 0) {
			//Codul nepotului
			afisarePID("nepot");
			exit(EXIT_SUCCESS);
		}
		printf("Procesul \033[32m%d\033[00m este zombie pentru %d sec\r\n", 
			pID3, (int)tm.tv_sec);

		nanosleep(&tm, NULL);	
		if (waitpid(pID3, &status, 0) == pID3) {
			printf("Procesul %d a fost inchis cod retur: %d\r\n", 
				(int)pID3, WEXITSTATUS(status)
			);
		}	
		exit(EXIT_SUCCESS);
	}
	
	// Crearea fiu 2
	pID2 = fork();
	if (pID2 == -1) {
		perror("esec la crearea fiului 2");
		exit(EXIT_FAILURE);
	}
	if (pID2 == 0) {
		// Codul fiului 1
		afisarePID("FIU 2");
		exit(EXIT_SUCCESS);
	}
		
	if (waitpid(pID2, &status, 0) == pID2) {
		printf("Procesul %d a fost inchis cod retur: %d\r\n", 
			(int)pID2, WEXITSTATUS(status)
		);
	}

	if (waitpid(pID1, &status, 0) == pID1) {
		printf("Procesul %d a fost inchis cod retur: %d\r\n", 
			(int)pID1, WEXITSTATUS(status)
		);
	}


	return 0;
}
