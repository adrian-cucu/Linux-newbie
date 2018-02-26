#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>


#define xx(__)  #__

#define author xx(Adrian Cucu)

#define program_name "dog"

#define Version	xx(0.001)	
#define Year xx (2017)

#define BUF_SIZE 	(1 << 20)

#define STREQ(s1, s2) (strcmp (s1, s2) == 0)

void emit_version ()
  {		
	printf (program_name": version " Version "\n"
			"Copyright (C) " Year "\n"
			"Written by " author "\n");

	exit (EXIT_SUCCESS);
}

void emit_helperf ()
  {
	system ("man cat");
	exit (EXIT_SUCCESS);
}

int main (int argc, char** argv){

	unsigned char out_buf [BUF_SIZE];
		
	setvbuf (stdout, out_buf, _IOFBF, BUF_SIZE);

	/* Option flags */
	int number_nonblank_lines = 0;
	int squeeze_blank_lines = 0;
	int line_number = 0;
	int show_tabs = 0;		
	int show_ends = 0;
	int show_nonprint = 0;
	int option_index = 0;
	/* Option flags */

	unsigned char buf [BUF_SIZE];

	int blank_line = 0;
	int new_line = 1;
	size_t n, ln = 1;
	int fd, opt;
	unsigned int c;	
	int i;
	struct stat st;
	char ** av;

	static struct option long_option [] = {

			{"version", no_argument, 0, 1},
			{"help", no_argument, 0, 2},
			{"show-all", no_argument, 0, 'A'},
			{"show-ends", no_argument, 0, 'E'},
			{"show-tabs", no_argument, 0, 'T'},
			{"show-nonprinting", no_argument, 0, 'v'},
			{"number", no_argument, 0, 'n'},
			{"number-nonblank", no_argument, 0, 'b'},
			{"squeeze-blank", no_argument, 0, 's'},
			{0, 0, 0, 0}
	};
	
	do
	  {
		opt = getopt_long (argc, argv, "bvsnTAE", 
							long_option, &option_index);	

		if (opt == -1) break;

		switch (opt)
			{
			case 1:	emit_version ();
			case 2: emit_helperf ();
			
			case 'A':  show_ends = 1;
					   show_tabs = 1;
				  	  break;
			case 'b':  number_nonblank_lines = 1;
					   line_number = 1;
					  break;
			case 'n':  line_number = 1; 			
				      break;
			case 'T':  show_tabs = 1;  				
					  break;			
			case 'E':  show_ends = 1;  				
					  break;
			case 'v':  show_nonprint = 1;			
					  break;				
			case 's':  squeeze_blank_lines = 1;		
			  		  break;
		}
	 }
	while (1); 		

	av = &argv [optind];

	do
	  {
		if (!*av || STREQ (*av, "-")) 
			fd = fileno (stdin);

		else
		    {
			  	if ( (fd = open (*av, O_RDONLY)) < 0)
			  	  {
					fprintf (stderr, program_name
							": %s: no such file or directory\n", *av);	
					goto __x;
				}

				fstat (fd, &st);

				if ((st.st_mode & S_IFMT) == S_IFDIR)
				  {
					fprintf (stderr, program_name
							": %s: Is a directory\n", *av);	
					goto __x;
				  }
		 }

		new_line = 1;

		while ((n = read (fd, buf, BUF_SIZE)))
		   {
			i = 0;
			while (i < n)
			   { 
				c = buf [i++];

				if (line_number && new_line)
				  {		
					if (!number_nonblank_lines || !blank_line)
						printf ("%6lu  ", ln++);
					new_line = 0;
				}

				if (isprint (c)) 
				  {
					putchar (c);
					blank_line = 0;
				 }
				else 
					switch (c){			
						case '\n':
							if (show_ends) 
								putchar ('$');
							putchar ('\n');
							
							if (buf[i] == '\n')
							  {		
								if (blank_line && squeeze_blank_lines)
								  {
									while (buf[i] == '\n')++i;
									blank_line = 0;
				  				  }
								else
									blank_line = 1;
							  }
							else
								blank_line = 0;
							new_line = 1;						
							 break;
						case '\t':	
							if (show_tabs)
								putchar ('^'), putchar ('I');
							else
								putchar ('\t');
						 	  break;
						default:
							if (!show_nonprint) 
								putchar (c);	
							else
							  {
								if (c & 0x80)
								  {
									putchar ('M');putchar ('-');
							   	  }
								else 
								  { 																				
									putchar ('^');
									c |= 0x40;
								  }
								c &= 0x7f;
								putchar (c);
						   	  }
				}	
			}
		}
		__x:
		if (! *av)	
			break;
		++av;
	  }	
	while (++optind < argc);	

	return EXIT_SUCCESS;}
