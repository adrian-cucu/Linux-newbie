/*
Sa se scrie in limbajul C un program numit infofis , 
care afiseaza informatii despre un fisier : 
lungimea fisierului, 
numele posesorului numele grupului si drepturile de acces. 
Afisarea se va face astfel :
Numele fisierului : f1.txt
Lungimea fisierului : 35 octeti
Posesor : c08ab
Grup : c08
Drepturi : Posesor – citire, scriere, executie
           Grup – citire executie
           Altii – citire
Indicatie: caracteristicile fisierului se pot afla 
cu functia stat sau prin executia (interna, in program !) 
a  comenzii ls , astfel:  exelp( “ls”, “ls”, “-l”,  “f1.txt”, “>f2.txt”, 0 ) ).
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// int stat(const char *pathname, struct stat *buf);

#define BUF_SIZE 512

          struct __stat {
               dev_t     st_dev;         /* ID of device containing file */
               ino_t     st_ino;         /* inode number */
               mode_t    st_mode;        /* protection */
               nlink_t   st_nlink;       /* number of hard links */
               uid_t     st_uid;         /* user ID of owner */
               gid_t     st_gid;         /* group ID of owner */
               dev_t     st_rdev;        /* device ID (if special file) */
               off_t     st_size;        /* total size, in bytes */
               blksize_t st_blksize;     /* blocksize for filesystem I/O */
               blkcnt_t  st_blocks;      /* number of 512B blocks allocated */

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */

               struct timespec st_atim;  /* time of last access */
               struct timespec st_mtim;  /* time of last modification */
               struct timespec st_ctim;  /* time of last status change */

           #define st_atime st_atim.tv_sec      /* Backward compatibility */
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };


int main(int argc, char** argv) 
{	
	int fd;
	char *cale_fis;
	char *nume_fisier;
	struct stat statistic;

	if (argc < 2) {
		fprintf(stderr, "usage %s: <nume fisier>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	cale_fis = argv[1];

	if (stat(cale_fis, &statistic) == -1) {
		perror("open");		
		exit(EXIT_FAILURE);
	}

	nume_fisier = strrchr(cale_fis, '/');
	nume_fisier = nume_fisier ? nume_fisier + 1 : cale_fis;
	mode_t mode = statistic.st_mode;

	printf("Numele fisierului : %s\n", nume_fisier);
	printf("Lungimea fisierului : %lu octeti\n", statistic.st_size);
	printf("Posesor : %s\n" , getpwuid (statistic.st_uid)-> pw_name);
	printf("Grup : %s\n", getpwuid (statistic.st_gid)-> pw_name);
	printf("Drepturi :");
	printf("\n\tPosesor – ");
	if (mode & S_IRUSR) printf("citire ");
	if (mode & S_IWUSR) printf("modificare ");
	if (mode & S_IXUSR) printf("executie");

	printf("\n\tGrup – ");
	if (mode & S_IRGRP) printf("citire ");
	if (mode & S_IWGRP) printf("modificare ");
	if (mode & S_IXGRP) printf("executie");

	printf("\n\tAltii – ");
	if (mode & S_IROTH) printf("citire ");
	if (mode & S_IWOTH) printf("modificare ");
	if (mode & S_IXOTH) printf("executie");
	printf("\n");
	fflush(stdout);
	
	return 0;
}
