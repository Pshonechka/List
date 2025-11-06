#include <stdio.h>
#include <stdlib.h>


#include "list.h"

int main() {
    List_t list = {};
    DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListInit (&list, 4); // (capacity)
//     list.data[1] = 10;
//     list.next[1] = 2;`
//     list.prev[1] = 0;
//     list.tail = 2;
//
//     list.data[2] = 20;
//     list.next[2] = 0;
//     list.prev[2] = 0;

    //list.free = 3;
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertAfter (&list, 0, 10);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertAfter (&list, 1, 20);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertAfter (&list, 0, 30);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertAfter (&list, 2, 40);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertBefore (&list, 4, 50);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);
    ListInsertAfter (&list, 4, 60);
    dump_info = {.fp = NULL, .macros_file = __FILE__, .macros_func = __func__, .macros_line = __LINE__};
    ListDump(&list, &dump_info);

    //constructor_of_struct(&list);

    DestructorOfList(&list);
    return 0;
}



