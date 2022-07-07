#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "filenode.h"

#define FILESIZE 4096
#define BLOCK 256

typedef Address List;

Address getLast(List L);

List createList(char * path);

int searchFile(char * text, char * pattern, int length);

int printFile(int fd);

#endif