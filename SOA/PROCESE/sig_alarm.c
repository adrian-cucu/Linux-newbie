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

int nbsec = 0;

void secunde(int sig) {
	alarm(1);
	nbsec++;
}
void intermed(int sig) {
	printf("Timp intermediar = %d sec\r\n", nbsec);
}
void Final(int sig) {
	printf("Timp final = %d sec\r\n", nbsec);
	exit(0);
}

int main (int argc, char** argv)
{
	signal(SIGALRM, secunde);
	signal(SIGINT, intermed);
	signal(SIGQUIT, Final);
	alarm(1);
	while (1)
		pause();
	return 0;
}
