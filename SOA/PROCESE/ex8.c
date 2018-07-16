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
#include <limits.h>

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



void handler(int sig, siginfo_t *sinfo, void *ptr)
{	
	printf("si_code: %d\r\n", sinfo->si_code);
	printf("si_pid: %d\r\n", sinfo->si_pid);
	printf("si_uid: %d\r\n", sinfo->si_uid);

	printf("----------------------------------\r\n");
}


int main (int argc, char** argv)
{
	pid_t fiu_1, fiu_2;
	struct timespec tm;
	int p[2];
	int status;
	char buffer[1001];
	int nbytes;
	char ch;
	/*
	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler;	
	sigaction(SIGCHLD, &sig, NULL);
	*/

	if (pipe(p) == -1) {
		perror("pipe(): ");
		exit(EXIT_FAILURE);
	}
	
	long pipe_size = fpathconf(p[0], _PC_PIPE_BUF);
	printf("pipe_size = %ld\r\n", pipe_size);
	return 0;
	
	fiu_1 = fork();
	if (fiu_1 == -1) error("eroare la crearea fiului 1");
	if (fiu_1 ==  0) {	
		close(p[0]);

		tm.tv_sec = 2;
		tm.tv_nsec = 0;
		ch = 'A';	
		
		while (1) {
			memset(buffer, ch, 1000);
			ch++;
			if (ch == 'Z' + 1) {
				ch = 'A';
			}	
			write(p[1], buffer, 1000);		
			//nanosleep(&tm, NULL);
		}
	
		exit(EXIT_SUCCESS);
	}
	fiu_2 = fork();
	if (fiu_2 == -1) error("eroare la crearea fiului 2");
	if (fiu_2 ==  0) {
		close(p[0]);

		tm.tv_sec = 2;
		tm.tv_nsec = 0;
		ch = 'z';	
		
		while (1) {
			memset(buffer, ch, 1000);
			ch--;
			if (ch == 'a' - 1) {
				ch = 'z';
			}	
			write(p[1], buffer, 1000);		
			//nanosleep(&tm, NULL);
		}
		

		exit(EXIT_SUCCESS);
	}

	close(p[1]);

	while ( (nbytes = read(p[0], buffer, 1000))) {
		write(1, buffer, nbytes);
		write(1, "\r\n\r\n", 4);	
	}
	

	if (waitpid(fiu_1, &status, 0) == fiu_1) {}
	if (waitpid(fiu_2, &status, 0) == fiu_2) {}

	/*
	if (WEXITSTATUS(status) == 0) {
		printf("Transfer efectuat!");
	}
	*/

	
	return 0;
}
