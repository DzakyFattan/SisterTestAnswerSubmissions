#include "filenode.h"

Address newNode(f_info info) {
    Address p = (Address) malloc(sizeof(Node));
    if (p != NULL) {
        INFO(p) = info;
        NEXT(p) = NULL;
    }
    return p;
}