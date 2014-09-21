#ifndef OBJ_MACRO_LIST_H_
#define OBJ_MACRO_LIST_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>

//
// madcrow_list.h
// Define a list with functions to alloc, reset, push, pop, shift, unshift etc.
//
// Example:
//
//   #include "madcrow_list.h"
//   madcrow_list_create(clist,CharList,char)
//
// Creates:
//
//   typedef struct {
//     char *data;
//     size_t len, capacity;
//   } CharList;
//
//   void   clist_alloc   (CharList *buf, size_t capacity)
//   void   clist_dealloc (CharList *buf)
//   void   clist_reset   (CharList *buf)
//   void   clist_capacity(CharList *buf, size_t capacity)
//   size_t clist_push    (CharList *buf, char obj)
//   char   clist_pop     (CharList *buf)
//   size_t clist_shift   (CharList *buf, char obj)
//   char   clist_unshift (CharList *buf)
//   char*  clist_get     (CharList *buf, size_t idx)
//   size_t clist_length  (const CharList *buf)
//
//  CharList clist = madcrow_list_init;
//  madcrow_list_verify(&clist);
//

// Round a number up to the nearest number that is a power of two
#ifndef leading_zeros
  #define leading_zeros(x) ((x) ? (__typeof(x))__builtin_clzll(x) : (__typeof(x))sizeof(x)*8)
#endif

#ifndef roundup2pow
  #define roundup2pow(x) (1UL << (64 - leading_zeros(x)))
#endif

#ifndef MC_MALLOC
  #define MC_MALLOC  malloc
#endif
#ifndef MC_REALLOC
  #define MC_REALLOC realloc
#endif


#define madcrow_list_init {.data = NULL, .start = 0, .end = 0, .capacity = 0}

#define madcrow_list_verify(list) do {                                         \
  assert(list->start <= list->end);                                            \
  assert(list->end <= list->capacity);                                         \
  assert(list->capacity == 0 || list->data != NULL);                           \
} while(0)

#define madcrow_list_create(FUNC,list_t,obj_t)                                 \
                                                                               \
typedef struct {                                                               \
  obj_t *data;                                                                 \
  size_t start, end, capacity;                                                 \
} list_t;                                                                      \
                                                                               \
/* Define functions with unused attribute in case they're not used */          \
static inline void FUNC ## _alloc(list_t *list, size_t capacity)               \
 __attribute__((unused));                                                      \
static inline void FUNC ## _dealloc(list_t *list)                              \
 __attribute__((unused));                                                      \
static inline void FUNC ## _capacity(list_t *list, size_t cap)                 \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _push(list_t *list, obj_t obj)                    \
 __attribute__((unused));                                                      \
static inline obj_t  FUNC ## _pop(list_t *list)                                \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _shift(list_t *list, obj_t obj)                   \
 __attribute__((unused));                                                      \
static inline obj_t  FUNC ## _unshift(list_t *list)                            \
 __attribute__((unused));                                                      \
static inline void FUNC ## _reset(list_t *list)                                \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _length(const list_t *list)                       \
 __attribute__((unused));                                                      \
static inline obj_t* FUNC ## _get(list_t *list, size_t idx)                    \
 __attribute__((unused));                                                      \
                                                                               \
static inline size_t FUNC ## _length(const list_t *list) {                     \
  madcrow_list_verify(list);                                                   \
  return list->end - list->start + 1;                                          \
}                                                                              \
                                                                               \
static inline obj_t* FUNC ## _get(list_t *list, size_t idx) {                  \
  assert(idx < list->end);                                                     \
  madcrow_list_verify(list);                                                   \
  return list->data + list->start + idx;                                       \
}                                                                              \
                                                                               \
static inline void FUNC ## _alloc(list_t *list, size_t capacity) {             \
  list->capacity = capacity < 8 ? 8 : roundup2pow(capacity);                   \
  list->data = MC_MALLOC(list->capacity * sizeof(obj_t));                      \
  list->start = list->end = list->capacity / 2;                                \
}                                                                              \
                                                                               \
static inline void FUNC ## _dealloc(list_t *list) {                            \
  madcrow_list_verify(list);                                                   \
  free(list->data);                                                            \
  memset(list, 0, sizeof(list_t));                                             \
}                                                                              \
                                                                               \
static inline void FUNC ## _capacity(list_t *list, size_t cap) {               \
  if(cap > list->capacity) {                                                   \
    cap = roundup2pow(cap);                                                    \
    list->data = MC_REALLOC(list->data, list->capacity * sizeof(obj_t));       \
    list->capacity = cap;                                                      \
  }                                                                            \
}                                                                              \
                                                                               \
/* Add an element to the end of the list */                                    \
static inline size_t FUNC ## _push(list_t *list, obj_t obj) {                  \
  madcrow_list_verify(list);                                                   \
  if(list->end >= list->capacity) {                                            \
    size_t n = FUNC ## _length(list);                                          \
    size_t half = list->capacity / 2;                                          \
    if(n >= half) {                                                            \
      list->capacity *= 2;                                                     \
      list->data = MC_REALLOC(list->data, list->capacity);                     \
    }                                                                          \
    else {                                                                     \
      size_t new_start = n; /* or = half - n */                                \
      memmove(list->data + new_start, list->data + list->start, n);            \
      list->start = new_start;                                                 \
      list->end = new_start + n;                                               \
    }                                                                          \
  }                                                                            \
  list->data[list->end++] = obj;                                               \
  return list->end - list->start - 1;                                          \
}                                                                              \
                                                                               \
/* Remove (and return) an element from the end of the list */                  \
static inline obj_t  FUNC ## _pop(list_t *list) {                              \
  assert(list->end > list->start);                                             \
  madcrow_list_verify(list);                                                   \
  return list->data[--list->end];                                              \
}                                                                              \
                                                                               \
/* Add an element to the start of the list */                                  \
static inline size_t FUNC ## _shift(list_t *list, obj_t obj) {                 \
  madcrow_list_verify(list);                                                   \
  if(list->start == 0) {                                                       \
    size_t n = FUNC ## _length(list);                                          \
    size_t half = list->capacity / 2;                                          \
    if(n >= half) {                                                            \
      list->capacity *= 2;                                                     \
      list->data = MC_REALLOC(list->data, list->capacity);                     \
    }                                                                          \
    size_t new_start = list->capacity - (2*n); /* or = half */                 \
    memmove(list->data + new_start, list->data + list->start, n);              \
    list->start = new_start;                                                   \
    list->end = new_start + n;                                                 \
  }                                                                            \
  list->data[--list->start] = obj;                                             \
  return 0;                                                                    \
}                                                                              \
                                                                               \
/* Remove (and return) an element from the start of the list */                \
static inline obj_t  FUNC ## _unshift(list_t *list) {                          \
  assert(list->start < list->end);                                             \
  madcrow_list_verify(list);                                                   \
  return list->data[list->start++];                                            \
}                                                                              \
                                                                               \
static inline void FUNC ## _reset(list_t *list) {                              \
  madcrow_list_verify(list);                                                   \
  list->start = list->end = list->capacity / 2;                                \
}

#endif /* OBJ_MACRO_LIST_H_ */
