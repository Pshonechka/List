#ifndef LIST_H
#define LIST_H

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define SPEC  "%14.2f"
typedef double item_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DPRINTF(...) printf(__VA_ARGS__)
#else
    #define DPRINTF(...) ;
#endif

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

const int    POISON_INT   = 0xBEBEBE;
const item_t POISON       = POISON_INT;
const size_t MIN_CAPACITY = 5;
const size_t MAX_CAPACITY = 100000;
const size_t MAX_SIZE     = 100000;

const double EPS = 1e-5;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef struct List {
    size_t size;
    size_t capacity;
    size_t free;
    // TODO: delete
    size_t head;
    size_t tail;
    size_t *next;
    size_t *prev;
    item_t *data;
} List_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef struct DumpVars {
    FILE *fp;
    const char *macros_file;
    const char *macros_func;
    int macros_line;
} DumpVars_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef enum ListErr {
    SUCCESS_LIST,
    NULL_LIST,
    NULL_DATA,
    NULL_NEXT,
    NULL_PREV,
    NULL_CAPACITY,
    CAPACITY_MORE_MAX,
    SIZE_MORE_MAX,
    SIZE_BIGGER_CAP,
    INDEX_NOT_IN_LIST,
    INDEX_MORE_CAPACITY,
    UNAVAILABLE_INDEX,
    OPENING_FILE
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

char const * const STR_ERRORS[] = {
    [SUCCESS_LIST] =        "List is ok",
    [NULL_LIST] =           "List ptr is null",
    [NULL_DATA] =           "Data is null",
    [NULL_NEXT] =           "Next is null",
    [NULL_PREV] =           "Prev is null",
    [NULL_CAPACITY] =       "Capacity is null",
    [CAPACITY_MORE_MAX] =   "Capacity more max",
    [SIZE_MORE_MAX] =       "Size more max",
    [SIZE_BIGGER_CAP] =     "Size bigger capacity",
    [INDEX_NOT_IN_LIST] =   "Index not in list",
    [INDEX_MORE_CAPACITY] = "Index more capacity",
    [UNAVAILABLE_INDEX] =   "Unavailable index",
    [OPENING_FILE] =        "Error in opening file"
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInit (List_t *list, size_t capacity);
int CompareDoubles (double a, double b);
ListErr_t DestructorOfList (List_t *list);
ListErr_t ListInsertAfter (List_t *list, size_t index, item_t el);
ListErr_t ListInsertBefore (List_t *list, size_t index, item_t el);
ListErr_t ListVerify (List_t *list);
ListErr_t ListRealloc (List_t *list);
ListErr_t ListDump (List_t *list, DumpVars_t *dump_info);
//int ListErrToStr (ListErr_t err, const char **line);


#endif // LIST_H
