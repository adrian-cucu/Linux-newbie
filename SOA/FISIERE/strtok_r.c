#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


int main (int argc, char **argv)
{
	char string[] = "Pene\nPeNE";	
	char *ptr;
	char *saveptr;

	while (ptr) {
		puts(ptr);
		puts(saveptr);
		ptr = strtok_r(NULL, "-", &saveptr);
	}

	if (*(saveptr - 1) == '\n') 
		printf("YES\n");
	

	return 0;
}
