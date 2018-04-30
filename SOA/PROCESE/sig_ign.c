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

void (*save_sig_handler)(int);

void h(int sig)
{
	printf("Signal: %d a fost primit\r\n", sig);
//	if (save_sig_handler)
		save_sig_handler(sig);
}


int main (int argc, char** argv)
{
	save_sig_handler =
	signal(SIGINT, h);
	while (1);

	//signal(SIGINT, SIG_IGN);
	//signal(SIGQUIT, SIG_IGN);

	return 0;
}
