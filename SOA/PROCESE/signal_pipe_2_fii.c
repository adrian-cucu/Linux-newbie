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
#include <signal.h>

void Final(int);

int p[2];

int main (int argc, char** argv)
{
	int f1, f2;
	char c;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	pipe(p);

	f1 = fork();
	if (f1 == 0) {
		close(p[0]);
		signal(SIGINT, Final);
		while (1)
			write(p[1], "A", 1);
	}
	f2 = fork();
	if (f2 == 0) {
		close(p[0]);
		signal(SIGQUIT, Final);
		while (1)
			write(p[1], "a", 1);
	}

	close(p[1]);

	while(read(p[0], &c, 1))
		write(1, &c, 1);

	write(1, "\n", 1);
	return 0;
}

void Final(int sig)
{
	if (sig == SIGINT)
		write(p[1], "Z", 1);
	else
		write(p[1], "z", 1);
	exit(1);
}
