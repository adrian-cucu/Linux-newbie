#include <stdlib.h>
#include <string.h>
#include <ctype.h>	/* islower() */
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char** argv) 
{
	int file_des ;   
	int i ;   
	char * nume_fis, *p_buf ;
	int t;
	
	printf("Tastati numele unui fisier text:  ");
	nume_fis = (char*) malloc(100);
	scanf("%s", nume_fis);

	file_des=open( nume_fis, O_RDONLY ) ;

	if (file_des == -1) {
		fprintf(stderr, "Eroare deschidere fisier!\n");
		return 1;
	}

	i = lseek(file_des, 0 , SEEK_END );

	if (i == -1) {
		fprintf(stderr, "Eroare lseek - 1\n");
		return 1;
	}

	if (lseek(file_des, 0 , SEEK_SET) == -1) {
		fprintf(stderr, "Eroare lseek - 2\n");
		return 1;
	}
	p_buf = (char*)malloc(i);
	
	if(read( file_des, p_buf, i) != i) {
		fprintf(stderr, "Eroare de citire fisier\n");
		return 1;
	}

	for (t = 0; t < i; ++ t) {
		if (islower(p_buf[t]))
			p_buf[t] = toupper(p_buf[t]);
		
	}
	
	
	if (write( 1, p_buf, i) != i)  {
		fprintf(stderr, "Eroare de scriere fisier\n");
		return 1;
	}
	


	return 0;
}
