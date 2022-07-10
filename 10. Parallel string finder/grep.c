#include <omp.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "filehandler.h"

int process_count;
int thread_count;

int main(int argc, char **args)
{

	char *str_in;
	DIR *dir;
	char buffer[FILESIZE];
	char *filename;
	Address P;
	List L;
	int child, i;
	f_info fd;

	if (argc != 4)
	{
		printf("Cara penggunaan : ./grep <jumlah process> <jumlah thread> <masukan string>\n");
		exit(0);
	}

	//*** INIT SECTION ***//
	process_count = atoi(args[1]);
	thread_count = atoi(args[2]);
	str_in = args[3];
	L = createList(".");
	int start = omp_get_wtime();

	//*** PARENT SECTION ***//
	P = L;
	child = 0;
	while (P)
	{
		pid_t cid;
		if ((cid = fork()) == 0)
		{
			goto child;
		}
		child++;
		if (child == process_count)
		{
			int stat;
			cid = wait(&stat);
			child--;
		}
		P = NEXT(P);
	}

	while (child > 0)
	{
		int stat;
		pid_t cid;
		cid = wait(&stat);
		child--;
	}

	printf("Pencarian selesai! Program telah berjalan selama %f detik.\n", omp_get_wtime() - start);
	exit(0);

child:
	fd = P->info;
	FILE *fp = fdopen(fd, "rb");
	fread(buffer, 1, FILESIZE, fp);
	int len = strlen(str_in);
	int found = 0;
	i = 0;

	omp_set_num_threads(thread_count);
	#pragma omp parallel default(none) shared(str_in, buffer, found, fd, len, i)
	#pragma omp single
	while (buffer[i] && !found)
	{
	#pragma omp task
		if (searchFile(&buffer[i], str_in, BLOCK))
		{
			if (!found)
			{
				found++;
				printFile(fd);
				printf("ProcessID: %d, Nomor Thread: %d\n", getpid(), omp_get_thread_num());
			}
		}
		i += BLOCK;
	}

	return 0;
}