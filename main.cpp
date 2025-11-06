#include <stdio.h>
#include <stdlib.h>


#include "list.h"

int main() {
    List_t list = {};
    DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListInit (&list, 4);

    ListInsertAfter (&list, 0, 10);
    ListInsertAfter (&list, 1, 20);
    ListInsertAfter (&list, 0, 30);
    ListInsertAfter (&list, 2, 40);
    ListInsertBefore (&list, 4, 50);
    ListInsertAfter (&list, 4, 60);

    DestructorOfList(&list);
    return 0;
}



