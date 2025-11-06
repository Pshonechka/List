#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "list.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define LIST_OK(list)                                                                           \
    do {                                                                                        \
        ListErr_t err = ListVerify(list);                                                      \
        if (err != SUCCESS_LIST) {                                                              \
            DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__,                        \
                                    .macros_func = __func__, .macros_line = __LINE__};          \
            ListDump (list, &dump_info);                                                        \
            return err;                                                                         \
        }                                                                                       \
        DPRINTF("meow1\n");                                                                     \
    } while (0)

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInsertAfter (List_t *list, size_t index,  item_t el) {
    LIST_OK(list);

    if (index >= list -> capacity) {
        return INDEX_MORE_CAPACITY;
    }
    if (CompareDoubles (list -> data[index], POISON) == 0){
        return INDEX_NOT_IN_LIST;
    }

    // Set new free
    size_t ins_pos = list -> free;
    list -> free = list -> next[list -> free];
    DPRINTF("free = %lu\n", list -> free);
    DPRINTF(SPEC"\n", el);
    list -> size += 1;

    if (list -> free == 0) {
        if (ListRealloc (list) == NULL_DATA) {
            return NULL_DATA;
        }
    }

    if (list -> next[0] == 0) {
        DPRINTF("\n!1! "SPEC"\n", el);
        list -> prev[0] = ins_pos;

        // Connect new element to prev and next
        list -> prev[ins_pos] = 0;
        list -> data[ins_pos] = el;
        list -> next[ins_pos] = 0;

        // Connect index_next to current
        list -> next[index] = ins_pos; // NOTE: index может быть только = 0

        // Connect index_prev to current
        list -> prev[list -> next[index]] = ins_pos;  // list->prev[0] = ins_pos (set tail)

        return SUCCESS_LIST;
    }

    DPRINTF("myau");
    //printf("myau");
    size_t index_next = list -> next[index];

    // Connect new element to prev and next
    list -> prev[ins_pos] = index;
    list -> data[ins_pos] = el;
    list -> next[ins_pos] = index_next;

    // Connect index_prev to current
    list -> prev[index_next] = ins_pos;

    // Connect index_next to current
    list -> next[index] = ins_pos;

    // TODO: Verify
    // TODO: call dump
    LIST_OK(list);
    return SUCCESS_LIST;

}

//—————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInsertBefore (List_t *list, size_t index, item_t el) {
    LIST_OK(list);
    ListInsertAfter (list, list -> prev[index], el);

    LIST_OK(list);

    return SUCCESS_LIST;
}
//
// // TODO: insert before = insert after(list->prev[index])
// ListErr_t ListInsertBefore (List_t *list, size_t index, item_t el) {
//     ListErr_t err = ListVerify (list);
//     if (err != SUCCESS_LIST) {
//         return err;
//     }
//     if (CompareDoubles (list -> data[index], POISON) == 0){
//         return INDEX_NOT_IN_LIST;
//     }
//     if (index >= list -> capacity) {
//         return INDEX_MORE_CAPACITY;
//     }
//     if (index == 0 || list -> tail == 0 || list -> head == 0) {
//         return UNAVAILABLE_INDEX;
//     }
//     size_t ins_pos = list -> free;
//     list -> free = list -> next[list -> free];
//     list -> data[ins_pos] = el;
//     // printf("!%lu\n", list -> prev[index]);
//   //  printf("!%lu\n", ins_pos);
//     list -> prev[ins_pos] = list -> prev[index];
//     list -> prev[index] = ins_pos;
//     list -> next[ins_pos] = index;
//   //  printf("!%lu\n", list -> next[list -> prev[ins_pos]]);
//     list -> next[list -> prev[ins_pos]] = ins_pos;
//     list -> head = list -> next[0];
//    // printf("!%lu\n", list -> next[index]);
//     list -> size += 1;
//     if (list -> size + 2 == list -> capacity) {
//         if (ListRealloc (list) == NULL_DATA) {
//             return NULL_DATA;
//         }
//     }
//
//     return SUCCESS_LIST;
// }

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

int CompareDoubles (double a, double b) {
    if (fabs(a-b) < EPS) {
        return 0;
    }
    else if (a-b > EPS) {
        return 1;
    }

    return -1;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInit (List_t *list, size_t capacity) {
    if (list == NULL) {
        return NULL_LIST;
    }
    if (list -> capacity > MAX_CAPACITY) {
        return CAPACITY_MORE_MAX;
    }
    if (capacity < MIN_CAPACITY) {
        capacity = MIN_CAPACITY;
    }

    list -> size = 0;
    list -> capacity = capacity;

    list -> data = (item_t *) calloc((list -> capacity), sizeof(item_t));
    if (list->data == NULL) {
        return NULL_DATA;
    }
    list -> next = (size_t*) calloc((list -> capacity), sizeof(size_t));
    if (list->next == NULL) {
        return NULL_NEXT;
    }
    list -> prev = (size_t*) calloc((list -> capacity), sizeof(size_t));
    if (list->prev == NULL) {
        return NULL_PREV;
    }

    for (size_t i = 1; i < list -> capacity; i++) {
        list -> data[i] = POISON;
        list -> prev[i] = POISON;
        list -> next[i] = i + 1;
    }

    list -> next[list -> capacity - 1] = 0;
    list -> free = 1;

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t DestructorOfList (List_t *list) {
    if (list == NULL) {
        return NULL_LIST;
    }
    free(list -> data);
    free(list -> next);
    free(list -> prev);

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListVerify (List_t *list) {
// TODO: free;
    if (list == NULL) {
        return NULL_LIST;
    }
// TODO: проверка массивов на логику
    if (list -> data == NULL) {
        return NULL_DATA;
    }
    if (list -> next == NULL) {
        return NULL_NEXT;
    }
    if (list -> prev == NULL) {
        return NULL_PREV;
    }
    if (list -> size > list -> capacity) {
        return SIZE_BIGGER_CAP;
    }
    if (list -> capacity > MAX_CAPACITY) {
        return CAPACITY_MORE_MAX;
    }
    if (list -> size > MAX_SIZE) {
        return SIZE_MORE_MAX;
    }

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListRealloc (List_t *list) {
    list -> capacity = (list -> capacity) * 2;

    item_t *re_data = (item_t *) realloc (list -> data, (list -> capacity) * sizeof(item_t));
    if (re_data == NULL) {
        return NULL_DATA;
    }
    list -> data = re_data;

    size_t *re_prev = (size_t *) realloc (list -> prev, (list -> capacity) * sizeof(size_t));
    if (re_prev == NULL) {
        return NULL_PREV;
    }
    list -> prev = re_prev;

    size_t *re_next = (size_t *) realloc (list -> next, (list -> capacity) * sizeof(size_t));
    if (re_next == NULL) {
        return NULL_NEXT;
    }
    list -> next = re_next;

    for (size_t i = list -> size + 1; i < list -> capacity; i++) {
        list -> data[i] = POISON;
        list -> prev[i] = POISON;
        list -> next[i] = i + 1;
    }

    list -> next[list -> capacity - 1] = 0;
    list -> free = list -> size + 1;

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListDump (List_t *list, DumpVars_t *dump_info) {
    static int calls_count = 0;
    calls_count++;
    ListErr_t err = ListVerify (list);
    const char *line = STR_ERRORS[err];
    //ListErrToStr (err, &line);

    DPRINTF("calls = %d\n", calls_count);

    FILE *fp = fopen("err.log", calls_count == 1 ? "w" : "a");

    if (fp == NULL) {
        DPRINTF("Error in opening file\n");
        return OPENING_FILE;
    }
    if (list == NULL) {
        fprintf (fp, "null list \n");
        return NULL_LIST;
    }

    fprintf (fp, "ListDump called from %s %s: %d \n%s \nlist [%p]\n\n",
             dump_info -> macros_file,
             dump_info -> macros_func,
             dump_info -> macros_line,
             line,
             list);

    fprintf (fp, "\t\t\t\t\t   ");
    for (size_t i = 0; i < list -> capacity; i++) {
        fprintf (fp, "%7lu", i);
    }

    fprintf (fp, "\ndata [%p] {", list -> data);
    for (size_t i = 0; i < list -> capacity; i++) {
        fprintf (fp, CompareDoubles (list -> data[i], POISON) == 0 ? "    PZN" : SPEC, list -> data[i]);
        //fprintf (fp, SPEC, list -> data[i]);
    }

    fprintf (fp, "}\nprev [%p] {", list -> prev);
    for (size_t i = 0; i < list -> capacity; i++) {
        fprintf (fp, list -> prev[i] == POISON_INT ? "    PZN" : "%7lu", list -> prev[i]);
        //fprintf (fp, "%14lu", list -> prev[i]);
    }

    fprintf (fp, "}\nnext [%p] {", list -> next);
    for (size_t i = 0; i < list -> capacity; i++) {
        fprintf (fp, "%7lu", list -> next[i]);
    }

    if (list -> data == NULL) {
        return NULL_DATA;
    }
    fprintf (fp, "}\n\n\nfree = %lu \nsize = %lu \ncapacity = %lu \n\n"
    "//——————————————————————————————————————————————————————————————————————————————————————————————————————————————\n\n",
            list -> free,
            list -> size,
            list -> capacity);
    //fprintf (fp, "\n\thead = %lu", list -> head);
    //fprintf (fp, "\n\ttail = %lu", list -> tail);
    // fprintf (fp, "\n\tsize = %lu", list -> size);
    // fprintf (fp, "\n\tcapacity = %lu \n", list -> capacity);
    // fprintf (fp, "\n}");
    // NOTE: fun fact
    // fprintf (fp, R"(
    // free = %lu
    // head = %lu
    // tail = %lu
    // size = %lu)");
    fclose(fp);

    return SUCCESS_LIST;
}
