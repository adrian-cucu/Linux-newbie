/*Doua procese fiu cicleaza la infinit trimitand in 
pipe buffere de cate 1000 de caractere.
Parintele afiseaza pe ecran carcatererle. Buferele nu trebuie sa se "amestece".
@Solutie data de Apostu Mihai - Nu prea functioneaza bine :)  poate o corectati!*/
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
	pid_t fiu, nepot1, nepot2;
	struct timespec tm;
	int i;
	int p[2];
	int status;
	char ch;
	/*
	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler;	
	sigaction(SIGCHLD, &sig, NULL);
	*/
	
	fiu = fork();
	if (fiu == 0) {
		if (pipe(p) == -1) {
			perror("pipe(): ");
			exit(1);
		}

		nepot1 = fork();
		if (nepot1 == 0) {
			close(p[0]);
			ch = 'A';
			for (i = 0; i < 500; ++ i) {
				write(p[1], &ch, 1);		
			}	
			exit(0);
		}
	
		nepot2 = fork();
		if (nepot2 == 0) {
			close(p[1]);	
			for (i = 0; i < 500; ++ i) {
				read(p[0], &ch, 1);		
				write(1, &ch, 1);
			}				
			write(1, "\n", 1);
			exit(0);
		}	

		
		waitpid(nepot2, &status, 0);	

		/*
		tm.tv_sec = 4;
		tm.tv_nsec = 0;		
		nanosleep(&tm, NULL);
		kill(getppid(), SIGUSR1);
		*/
		exit(0);
	}
	
	printf("Parent\r\n");	
	wait(&status);

	if (WEXITSTATUS(status) == 0) {
		printf("Transfer efectuat!");
	}

	
	return 0;
}
