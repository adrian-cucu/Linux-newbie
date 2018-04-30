/*
1. Să se realizeze în limbajul C un program numit parent0 
care să creeze trei procese(inclusiv părintele). 
Fiecare proces își afișează PID-ul, 
primul proces fiu numără de la 1-50000, 
al doilea proces fiu de la 50000-100000, 
iar procesul părinte afișează literele din alfabet 
în mod continuu până când ambele procese fiu își 
încheie execuția, moment în care la
rândul lui își va încheia execuția. 
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
	pid_t pID, pID1, pID2;
	int cnt;
	int status;
	int kids;
	struct timespec tm = {0, 500000000};

	printf("Parinte PID: %d\r\n", getpid());

	// Crearea fiu 1
	pID1 = fork();
	if (pID1 == -1) {
		perror("esec la crearea fiului 1");
		exit(EXIT_FAILURE);
	}
	if (pID1 == 0) {
		// Codul fiului 1
		printf("Fiu #1 PID: %d\r\n", getpid());
		for (cnt = 1; cnt <= 50000; ++cnt) {
			printf("%d\r\n", cnt);	
		//	nanosleep(&tm ,NULL);
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
		printf("Fiu #2 PID: %d\r\n", getpid());
		for (cnt = 50000; cnt <= 100000; ++cnt) {
			printf("%d\r\n", cnt);
		//	nanosleep(&tm ,NULL);
		}
		exit(EXIT_SUCCESS);
	}
	
	// Codul procesului tata
	kids = 2;
	cnt = 'A';
	while (kids) {
		printf("%c\r\n", cnt);

		pID = waitpid(-1, &status, WNOHANG);
		if (pID > 0) {
			--kids;
		} 

		++cnt;
		if (cnt == 'Z') cnt = 'A';

		//nanosleep(&tm ,NULL);
	}		

	
	return 0;
}
