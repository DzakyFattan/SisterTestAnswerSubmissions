#ifndef FILENODE_H
#define FILENODE_H

#include <stdlib.h>

typedef int f_info;
typedef struct node* Address;
typedef struct node {
    f_info info;
    Address next;
} Node;

#define INFO(p) (p)->info
#define NEXT(p) (p)->next

Address newNode(f_info info);

#endif