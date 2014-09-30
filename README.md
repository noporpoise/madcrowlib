MadCrowLib
==========
Isaac Turner <turner.isaac@gmail.com>  
License: MIT  

MadCrowLib is a library of stand alone header files that provide macros for common data structures in C.

"mad crow" because it sounds a bit like "macro"

You can override the memory allocators used with:

    #define MC_CALLOC  calloc2
    #define MC_REALLOC realloc2

madcrow_buffer.h
----------------

Define a buffer with functions to alloc, resize, add, append, reset etc.

Example:

    #include "madcrow_buffer.h"
    madcrow_buffer_create(charbuf,String,char)

Creates:

    typedef struct {
      char *data;
      size_t len, capacity;
    } String;

    void    charbuf_alloc       (String *buf, size_t capacity)
    void    charbuf_dealloc     (String *buf)
    void    charbuf_reset       (String *buf)
    void    charbuf_capacity    (String *buf, size_t capacity)
    size_t  charbuf_add         (String *buf, char obj)
    ssize_t charbuf_attempt_add (String *buf, char obj)
    void    charbuf_append      (String *buf, char *obj, size_t n)
    void    charbuf_shift_left  (String *buf, size_t n)
    void    charbuf_shift_right (String *buf, size_t n)

We also provide general macros to create an empty (unallocated) buffer, and a
macro to verify that a buffer is a valid structure:

    String string = madcrow_buffer_init;
    madcrow_buffer_verify(&string);


madcrow_list.h
--------------

Define a list with functions to alloc, reset, push, pop, shift, unshift etc.

Example:

    #include "madcrow_list.h"
    madcrow_list_create(clist,CharList,char)

Creates:

    typedef struct {
      char *data;
      size_t len, capacity;
    } CharList;

    void   clist_alloc   (CharList *buf, size_t capacity)
    void   clist_dealloc (CharList *buf)
    void   clist_reset   (CharList *buf)
    void   clist_capacity(CharList *buf, size_t capacity)
    size_t clist_push    (CharList *buf, char obj)
    char   clist_pop     (CharList *buf)
    void   clist_popn    (CharList *buf, size_t n)
    size_t clist_unshift (CharList *buf, char obj)
    char   clist_shift   (CharList *buf)
    void   clist_shiftn  (CharList *buf, size_t n)
    char*  clist_get     (CharList *buf, size_t idx)
    size_t clist_length  (const CharList *buf)

We also provide general macros to create an empty (unallocated) list, and a
macro to verify that a list is a valid structure:

    CharList clist = madcrow_list_init;
    madcrow_list_verify(&clist);


madcrow_linkedlist.h
--------------------

Define a doubly-linked list with functions to push, pop, shift, unshift etc.

Example:

    #include "madcrow_linkedlist.h"
    madcrow_linkedlist_create(llist,LinkedList,LinkedNode,char)

Creates:

    typedef struct {
      char data;
      LinkedNode *next, *prev;
    } LinkedNode;

    typedef struct {
      LinkedNode *first, *last;
    } LinkedList;

    void        llist_init    (LinkedList *llist)
    void        llist_push    (LinkedList *llist, LinkedNode *obj)
    LinkedNode* llist_pop     (LinkedList *llist)
    void        llist_shift   (LinkedList *llist, LinkedNode *obj)
    LinkedNode* llist_unshift (LinkedList *llist)
    size_t      llist_length  (const LinkedList *llist)

We also provide general macros to create an empty (unallocated) list, and a
macro to verify that a linkedlist is a valid structure:

    LinkedList llist = madcrow_linkedlist_init;
    madcrow_linkedlist_verify(&llist);

