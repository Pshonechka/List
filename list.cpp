#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "list.h"

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
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
#else
    #define  LIST_OK(...) ;
#endif

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


    if (list -> next[0] == 0) {
        DPRINTF("\n!1! "SPEC"\n", el);

        // Connect index_prev to current
        list -> prev[list -> next[index]] = ins_pos;  // list->prev[0] = ins_pos (set tail)

        // Connect index_next to current
        list -> next[index] = ins_pos; // NOTE: index может быть только = 0

        // Connect new element to prev and next
        list -> prev[ins_pos] = 0;
        list -> data[ins_pos] = el;
        list -> next[ins_pos] = 0;

        // List dump
        DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__,
                            .macros_func = __func__, .macros_line = __LINE__};
        ListDump(list, &dump_info);

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

    LIST_OK(list);

    DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__,
                            .macros_func = __func__, .macros_line = __LINE__};
    ListDump(list, &dump_info);

    if (list -> free == 0) {
        if (ListRealloc (list) == NULL_DATA) {
            return NULL_DATA;
        }
    }

    return SUCCESS_LIST;

}

//—————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInsertBefore (List_t *list, size_t index, item_t el) {
    LIST_OK(list);

    ListInsertAfter (list, list -> prev[index], el);

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t DeleteElementFromList (List_t *list, size_t index) {
    LIST_OK(list);
    if (index >= list -> capacity) {
        return INDEX_MORE_CAPACITY;
    }
    if (index < 1) {
        return UNAVAILABLE_INDEX;
    }
    if (CompareDoubles (list -> data[index], POISON) == 0){
        return INDEX_NOT_IN_LIST;
    }

    // Connect prev[ind] and next_ind with each other
    list -> next[list -> prev[index]] = list -> next[index];
    list -> prev[list -> next[index]] = list -> prev[index];

    // Set new free
    list -> next[index] = list -> free;
    list -> free = index;

    // Poison deleted element
    list -> data[index] = POISON;
    list -> prev[index] = POISON;

    LIST_OK(list);

    DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__,
                            .macros_func = __func__, .macros_line = __LINE__};
    ListDump(list, &dump_info);

    return SUCCESS_LIST;
}

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

    LIST_OK(list);

    DumpVars_t dump_info = {.fp = NULL, .macros_file = __FILE__,
                            .macros_func = __func__, .macros_line = __LINE__};
    ListDump(list, &dump_info);

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

    if (list == NULL) {
        return NULL_LIST;
    }

    size_t size = list -> size;
    size_t capacity = list -> capacity;
    size_t *next = list -> next;
    size_t *prev = list -> prev;
    item_t *data = list -> data;

    if (data == NULL) {
        return NULL_DATA;
    }
    if (next == NULL) {
        return NULL_NEXT;
    }
    if (prev == NULL) {
        return NULL_PREV;
    }

    for (size_t i = 0; i < capacity; i++) {
        if (prev[i] != POISON_INT && prev[i] > capacity) {
            return PREV_WRONG_VALUE;
        }
        if (next[i] > capacity) {
            return NEXT_WRONG_VALUE;
        }
    }
    size_t *next_cnt = (size_t *) calloc((list -> capacity + 1), sizeof(size_t));
    size_t *prev_cnt = (size_t *) calloc((list -> capacity + 1), sizeof(size_t));
    size_t *next_poison_cnt = (size_t *) calloc((list -> capacity + 1), sizeof(size_t));

    for (size_t i = 0; i < capacity; i++) {
        if (CompareDoubles(data[i], POISON) == 0) {
            next_poison_cnt[next[i]] += 1;
        }
        else {
            next_cnt[next[i]] += 1;
            prev_cnt[prev[i]] += 1;
        }
    }

    for (size_t i = 0; i < capacity; i++) {

        if (next_cnt[i] > 1) {
            free(next_cnt);
            free(prev_cnt);
            free (next_poison_cnt);
            return CYCLED_NEXT_VALUE;
        }
        if (prev_cnt[i] > 1) {
            free(next_cnt);
            free(prev_cnt);
            free (next_poison_cnt);
            return CYCLED_PREV_VALUE;
        }
        if (next_poison_cnt[i] > 1) {
            free(next_cnt);
            free(prev_cnt);
            free (next_poison_cnt);
            return CYCLED_FREE;
        }
    }

    free(prev_cnt);
    free(next_cnt);
    free (next_poison_cnt);

    for (size_t i = 0; i < capacity; i++) {
        if (CompareDoubles(data[i], POISON) != 0) {
            if (prev[next[i]] != next[prev[i]]) {
                return INCORRECT_CONNECTIONS;
            }
        }
    }

    if (size > capacity) {
        return SIZE_BIGGER_CAP;
    }
    if (capacity > MAX_CAPACITY) {
        return CAPACITY_MORE_MAX;
    }
    if (size > MAX_SIZE) {
        return SIZE_MORE_MAX;
    }

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListRealloc (List_t *list) {
    LIST_OK(list);

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

    LIST_OK(list);

    return SUCCESS_LIST;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListDump (List_t *list, DumpVars_t *dump_info) {
    static int calls_count = 0;
    calls_count++;
    ListErr_t err = ListVerify (list);
    const char *line = STR_ERRORS[err];

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
    }

    fprintf (fp, "}\nprev [%p] {", list -> prev);
    for (size_t i = 0; i < list -> capacity; i++) {
        fprintf (fp, list -> prev[i] == POISON_INT ? "    PZN" : "%7lu", list -> prev[i]);
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

    fclose(fp);

    return SUCCESS_LIST;
}
