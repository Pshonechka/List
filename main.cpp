#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
    List_t list = {};
    ListInit (&list, 4);

    ListInsertAfter (&list, 0, 10);
    ListInsertAfter (&list, 1, 20);
    ListInsertAfter (&list, 0, 30);
    ListInsertAfter (&list, 0, 40);
    ListInsertBefore (&list, 0, 50);
    ListInsertAfter (&list, 4, 60);

    DestructorOfList(&list);
    return 0;
}



