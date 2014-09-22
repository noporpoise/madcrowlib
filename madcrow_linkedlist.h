#ifndef MADCROW_LINKEDLIST_H_
#define MADCROW_LINKEDLIST_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>

//
// madcrow_linkedlist.h
// Define a doubly-linked list with functions to push, pop, shift, unshift etc.
//
// Example:
//
//   #include "madcrow_linkedlist.h"
//   madcrow_linkedlist_create(llist,LinkedList,LinkedNode,char)
//
// Creates:
//
//   typedef struct {
//     char data;
//     LinkedNode *next, *prev;
//   } LinkedNode;
//
//   typedef struct {
//     LinkedNode *first, *last;
//   } LinkedList;
//
//   void        llist_init    (LinkedList *llist)
//   void        llist_push    (LinkedList *llist, LinkedNode *obj)
//   LinkedNode* llist_pop     (LinkedList *llist)
//   void        llist_shift   (LinkedList *llist, LinkedNode *obj)
//   LinkedNode* llist_unshift (LinkedList *llist)
//   size_t      llist_length  (const LinkedList *llist)
//
//  LinkedList llist = madcrow_linkedlist_init;
//  madcrow_linkedlist_verify(&llist);
//

#define madcrow_linkedlist_init {.first = NULL, .last = NULL, .len = 0}

#define madcrow_linkedlist_verify(list) do {                                   \
  assert(!(list)->first == !(list)->last);                                     \
  assert(!(list)->first == !(list)->len);                                      \
  { size_t _n = 0; __typeof((list)->first) _ptr = (list)->first;               \
    while(_ptr) { _ptr = _ptr->next; _n++; }                                   \
    assert(_n == (list)->len);                                                 \
  }                                                                            \
} while(0)

#define madcrow_linkedlist(FUNC,list_t,node_t,obj_t)                           \
                                                                               \
typedef struct __##node_t node_t;                                              \
                                                                               \
struct __##node_t {                                                            \
  obj_t data;                                                                  \
  node_t *next, *prev;                                                         \
};                                                                             \
                                                                               \
typedef struct {                                                               \
  node_t *first, *last;                                                        \
  size_t len;                                                                  \
} list_t;                                                                      \
                                                                               \
/* Define functions with unused attribute in case they're not used */          \
static inline void FUNC ## _init(list_t *list)                                 \
 __attribute__((unused));                                                      \
static inline void FUNC ## _push(list_t *list, node_t *node)                   \
 __attribute__((unused));                                                      \
static inline node_t*  FUNC ## _pop(list_t *list)                              \
 __attribute__((unused));                                                      \
static inline void FUNC ## _shift(list_t *list, node_t *node)                  \
 __attribute__((unused));                                                      \
static inline node_t*  FUNC ## _unshift(list_t *list)                          \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _length(const list_t *list)                       \
 __attribute__((unused));                                                      \
                                                                               \
static inline void FUNC ## _init(list_t *list) {                               \
  memset(list, 0, sizeof(list_t));                                             \
}                                                                              \
                                                                               \
static inline size_t FUNC ## _length(const list_t *list) {                     \
  node_t *n = list->first; size_t len = 0;                                     \
  while(n != NULL) { n++; len++; }                                             \
  return len;                                                                  \
}                                                                              \
                                                                               \
/* Add an element to the end of the list */                                    \
static inline void FUNC ## _push(list_t *list, node_t *node) {                 \
  if(list->last) list->last->next = node;                                      \
  else           list->first = node;                                           \
  node->next = NULL;                                                           \
  node->prev = list->last;                                                     \
  list->last = node;                                                           \
}                                                                              \
                                                                               \
/* Remove (and return) an element from the end of the list */                  \
static inline node_t* FUNC ## _pop(list_t *list) {                             \
  node_t *node = list->last;                                                   \
  list->last = list->last->prev;                                               \
  if(list->last == NULL) list->first = NULL;                                   \
  return node;                                                                 \
}                                                                              \
                                                                               \
/* Add an element to the start of the list */                                  \
static inline void FUNC ## _shift(list_t *list, node_t *node) {                \
  if(list->first) list->first->prev = node;                                    \
  else            list->last = node;                                           \
  node->next = list->first;                                                    \
  node->prev = NULL;                                                           \
  list->first = node;                                                          \
}                                                                              \
                                                                               \
/* Remove (and return) an element from the start of the list */                \
static inline node_t* FUNC ## _unshift(list_t *list) {                         \
  node_t *node = list->first;                                                  \
  list->first = list->first->next;                                             \
  if(list->first == NULL) list->last = NULL;                                   \
  return node;                                                                 \
}

#endif /* MADCROW_LINKEDLIST_H_ */
