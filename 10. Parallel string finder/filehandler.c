#include "filehandler.h"

List createList(char *path)
{
	List L = NULL;
	DIR *d = opendir(path);
	if (d == NULL)
	{
		return L;
	}
	struct dirent *entry;
	Address P = newNode(0);
	Address P_start = P;
	char dir_path[512];
	while ((entry = readdir(d)) != NULL)
	{
		sprintf(dir_path, "%s/%s", path, entry->d_name);
		if (entry->d_type != DT_DIR)
		{
			NEXT(P) = newNode(fileno(fopen(dir_path, "r")));
			P = NEXT(P);
			if (!L)
				L = P;
		}
		else if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
		{
			NEXT(P) = createList(dir_path);
			if (NEXT(P))
			{
				P = getLast(NEXT(P));
				if (!L)
					L = NEXT(P);
			}
		}
	}
	free(P_start);
	closedir(d);
	return L;
}

Address getLast(List L)
{
	Address P = L;
	while (NEXT(P))
		P = NEXT(P);
	return P;
}

int searchFile(char *text, char *pattern, int length)
{
	int i, j;
	for (i = 0; i < length; i++)
	{
		for (j = 0; pattern[j] && text[i + j]; j++)
		{
			if (text[i + j] != pattern[j])
				break;
		}
		if (pattern[j] == '\0')
			return 1;
	}
	return 0;
}

int printFile(int fd)
{
	char filePath[BLOCK];
	char result[BLOCK];
	sprintf(filePath, "/proc/self/fd/%d", fd);
	memset(result, 0, sizeof(result));
	readlink(filePath, result, sizeof(result));
	printf("%s\n", result);
	return fd;
}