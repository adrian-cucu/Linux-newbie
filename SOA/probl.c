/*
Doua procese fiu cicleaza la infinit trimitand in 
pipe buffere de cate 1000 de caractere.
Parintele afiseaza pe ecran carcatererle. Buferele nu trebuie sa se "amestece".
@Solutie data de Apostu Mihai - Nu prea functioneaza bine :)  poate o corectati!
*/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define BUF_SIZE 1000

int lock(int fd) {
	struct flock    lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	return fcntl(fd, F_SETLKW, &lock);
}

int unlock(int fd)  {
	struct flock    lock;
	lock.l_type = F_UNLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	return fcntl(fd,F_SETLKW,&lock);
}

int main (int argc, char** argv)
{
	char buf[BUF_SIZE + 1];
	pid_t pID1, pID2;
	int p[2];
	size_t n;

	if (pipe(p) == -1) {
		perror("pipe():");
		exit(EXIT_FAILURE);
	}	

	pID1 = fork();

	if (pID1 == -1) {
		/*Programul se va incheia*/
		perror("[1]fork()");	
		exit(EXIT_FAILURE);
	}	

	if (pID1 == 0) {
		/* Primul process fiu */
		printf("-- Procesul fiu #1 PID[%d]\n", getpid());
		close(p[0]); /* Inchidem capatul de citire */
		memset(buf, 'A', BUF_SIZE);

		while (1) {
			lock(p[1]);
			printf("Capat pipe blocat pe #1\n");		
			write(p[1], buf, BUF_SIZE);
			unlock(p[1]);	
			printf("Capat pipe deblocat pe #1\n");		
			fflush(stdout);
			sleep(1);
		}

		exit(EXIT_SUCCESS);
	}
	
	pID2 = fork();

	if (pID2 == -1) {
		perror("[2]fork()");	
		exit(EXIT_FAILURE);
	}	

	if (pID2 == 0) {
		/* Al 2-lea process fiu */
		printf("-- Procesul fiu #2 PID[%d]\n", getpid());
		close(p[0]); /* Inchidem capatul de citire */
		memset(buf, 'B', BUF_SIZE);

		while (1) {
			lock(p[1]);
			printf("Capat pipe blocat pe #2\n");		
			write(p[1], buf, BUF_SIZE);
			unlock(p[1]);	
			printf("Capat pipe deblocat pe #2\n");
			fflush(stdout);
			sleep(1);
		}

		exit(EXIT_SUCCESS);
	}

	/* Restul programului este executat doar de procesul parinte */

	printf("-- Parinte PID(%d) cu fii %d %d\n", getpid(), pID1, pID2);

	close(p[1]); /* Inchidem capatul de scriere */

	while (1) {
		if ((n = read(p[0], buf, BUF_SIZE))) {
			write(STDOUT_FILENO, buf, n);
		}
	}
	
	return 0;
}
