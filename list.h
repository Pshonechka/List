#ifndef LIST_H
#define LIST_H

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define SPEC        "%7.2f"
#define SPEC_GRAPH  "%2.2f"
typedef double item_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DPRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
    #define DPRINTF(...) ;
#endif

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

const int    POISON_INT   = 0xBEBEBE;
const item_t POISON       = POISON_INT;


const double EPS = 1e-5;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef struct List {
    size_t size;
    size_t capacity;
    size_t free;
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
    FREE_MORE_CAPACITY,
    NULL_CAPACITY,
    NULL_FILE,
    SIZE_BIGGER_CAP,
    INDEX_NOT_IN_LIST,
    INDEX_MORE_CAPACITY,
    UNAVAILABLE_INDEX,
    OPENING_FILE,
    PREV_WRONG_VALUE,
    NEXT_WRONG_VALUE,
    CYCLED_FREE,
    CYCLED_NEXT_VALUE,
    CYCLED_PREV_VALUE,
    INCORRECT_CONNECTIONS
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

char const * const LIST_STR_ERRORS[] = {
    [SUCCESS_LIST]            = "List is ok",
    [NULL_LIST]               = "List ptr is null",
    [NULL_DATA]               = "Data is null",
    [NULL_NEXT]               = "Next is null",
    [NULL_PREV]               = "Prev is null",
    [FREE_MORE_CAPACITY]      = "Free more capacity",
    [NULL_CAPACITY]           = "Capacity is null",
    [NULL_FILE]               = "Null pointer to file",
    [SIZE_BIGGER_CAP]         = "Size bigger capacity",
    [INDEX_NOT_IN_LIST]       = "Index not in list",
    [INDEX_MORE_CAPACITY]     = "Index more capacity",
    [UNAVAILABLE_INDEX]       = "Unavailable index",
    [OPENING_FILE]            = "Error in opening file",
    [PREV_WRONG_VALUE]        = "Prev[value] > capacity",
    [NEXT_WRONG_VALUE]        = "Next[value] > capacity",
    [CYCLED_FREE]             = "Several next[free] are equal",
    [CYCLED_NEXT_VALUE]       = "Several next[value] are equal",
    [CYCLED_PREV_VALUE]       = "Several prev[value] are equal",
    [INCORRECT_CONNECTIONS]   = "Elements are connected only in one direction"
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListInit (List_t *list, size_t capacity);
int CompareDoubles (double a, double b);
ListErr_t DestructorOfList (List_t *list);
ListErr_t ListInsertAfter (List_t *list, size_t index, item_t el);
ListErr_t ListInsertBefore (List_t *list, size_t index, item_t el);
ListErr_t DeleteElementFromList (List_t *list, size_t index);
ListErr_t ListVerify (List_t *list);
ListErr_t ListRealloc (List_t *list);
ListErr_t ListDump (List_t *list, DumpVars_t *dump_info);
ListErr_t GraphDump (List_t *list);
ListErr_t MakeNode(List_t *list, size_t index, FILE *fp);
ListErr_t InvisibleConnections (List_t *list, FILE *fp);
ListErr_t NextConnections (List_t *list, FILE *fp);
ListErr_t PrevConnections (List_t *list, FILE *fp);
ListErr_t HeadTail (List_t *list, FILE *fp);

#endif // LIST_H
