#ifndef MADCROW_LIST_H_
#define MADCROW_LIST_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>
#include <unistd.h> // ssize_t
#include <inttypes.h> // uint64_t

//
// madcrow_list.h
// Define a list with functions to alloc, reset, push, pop, shift, unshift etc.
//
// Example:
//
//   #include "madcrow_list.h"
//   madcrow_list(clist,CharList,char)
//
// Creates:
//
//   typedef struct {
//     char *b;
//     size_t start, end, capacity;
//   } CharList;
//
//   CharList* clist_new     (size_t capacity)
//   void      clist_destroy (CharList *list)
//   void      clist_alloc   (CharList *list, size_t capacity)
//   void      clist_dealloc (CharList *list)
//   void      clist_reset   (CharList *list)
//   void      clist_capacity(CharList *list, size_t capacity)
//   void      clist_len     (const CharList *list)
//
// Pass object:
//   size_t    clist_prepend  (CharList *list, char obj)
//   size_t    clist_append   (CharList *list, char obj)
//   char      clist_lcut     (CharList *list)
//   char      clist_rcut     (CharList *list)
//   char      clist_get      (CharList *list, size_t idx)
//   void      clist_set      (CharList *list, size_t idx, char obj)
//
// Pass pointers:
//   void       clist_getn    (CharList *list, size_t idx, char *ptr, size_t n)
//   void       clist_setn    (CharList *list, size_t idx,
//                             char const *ptr, size_t n)
//   char*     clist_getptr   (CharList *list, size_t idx)
//   cosnt char* clist_getconstptr(const CharList *list, size_t idx)
//
//   size_t     clist_push    (CharList *list, char const *ptr, size_t n)
//   void       clist_pop     (CharList *list, char *ptr, size_t n)
//   void       clist_unshift (CharList *list, char const *ptr, size_t n)
//   void       clist_shift   (CharList *list, char *ptr, size_t n)
//
//   void       clist_copy    (CharList *dst, const CharList *src)
//
//  CharList clist = madcrow_list_init;
//  madcrow_list_verify(&clist);
//

// Round a number up to the nearest number that is a power of two
#ifndef leading_zeros
  #define leading_zeros(x) ((x) ? (__typeof(x))__builtin_clzll(x) \
                                : (__typeof(x))sizeof(x)*8)
#endif

#ifndef roundup2pow
  #define roundup2pow(x) (1UL << (64 - leading_zeros((uint64_t)(x))))
#endif

#define madcrow_list_init {.b = NULL, .start = 0, .end = 0, .capacity = 0}

#define madcrow_list_verify(list) do {                                         \
  assert(list->start <= list->end);                                            \
  assert(list->end <= list->capacity);                                         \
  assert(list->capacity == 0 || list->b != NULL);                              \
} while(0)

#define madcrow_list(name,list_t,obj_t) \
        madcrow_list2(name,list_t,obj_t,calloc,realloc,free)

#define madcrow_list2(FUNC,list_t,obj_t,mc_alloc,mc_realloc,mc_free)           \
                                                                               \
typedef struct {                                                               \
  obj_t *b;                                                                    \
  size_t start, end, capacity;                                                 \
} list_t;                                                                      \
                                                                               \
/* Define functions with unused attribute in case they're not used */          \
static inline list_t* FUNC ## _new(size_t capacity)                            \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _destroy(list_t *list)                           \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _alloc(list_t *list, size_t capacity)            \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _dealloc(list_t *list)                           \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _capacity(list_t *list, size_t cap)              \
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _push(list_t *list, const obj_t *obj, size_t n)  \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _pop(list_t *list, obj_t *ptr, size_t n)         \
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _unshift(list_t *list, const obj_t *ptr, size_t n)\
 __attribute__((unused));                                                      \
static inline void    FUNC ## _shift(list_t *list, obj_t *ptr, size_t n)       \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _reset(list_t *list)                             \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _copy(list_t *dst, const list_t *src)            \
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _len(const list_t *list)                         \
 __attribute__((unused));                                                      \
static inline obj_t   FUNC ## _get(list_t *list, size_t idx)                   \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _set(list_t *list, size_t idx, obj_t obj)        \
 __attribute__((unused));                                                      \
static inline obj_t*   FUNC ## _getptr(list_t *list, size_t idx)               \
 __attribute__((unused));                                                      \
static inline const obj_t*   FUNC ## _getconstptr(const list_t *list, size_t idx)\
 __attribute__((unused));                                                      \
\
static inline size_t  FUNC ## _prepend(list_t *list, obj_t obj)                \
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _append(list_t *list, obj_t obj)                 \
 __attribute__((unused));                                                      \
static inline obj_t   FUNC ## _lcut(list_t *list)                              \
 __attribute__((unused));                                                      \
static inline obj_t   FUNC ## _rcut(list_t *list)                              \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _getn(list_t *list, size_t idx,                  \
                                    obj_t *ptr, size_t n)                      \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _setn(list_t *list, size_t idx,                  \
                                    const obj_t *ptr, size_t n)                \
 __attribute__((unused));                                                      \
                                                                               \
static inline list_t* FUNC ## _new(size_t capacity) {                          \
  list_t *l = mc_alloc(1, sizeof(list_t));                                     \
  if(l) FUNC ## _alloc(l, capacity);                                           \
  return l;                                                                    \
}                                                                              \
                                                                               \
static inline void    FUNC ## _destroy(list_t *list) {                         \
  FUNC ## _dealloc(list);                                                      \
  mc_free(list);                                                               \
}                                                                              \
                                                                               \
static inline size_t  FUNC ## _len(const list_t *list) {                       \
  madcrow_list_verify(list);                                                   \
  return list->end - list->start;                                              \
}                                                                              \
                                                                               \
static inline obj_t   FUNC ## _get(list_t *list, size_t idx) {                 \
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  return list->b[list->start + idx];                                           \
}                                                                              \
                                                                               \
static inline obj_t*  FUNC ## _getptr(list_t *list, size_t idx) {              \
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  return list->b + list->start + idx;                                          \
}                                                                              \
                                                                               \
static inline const obj_t*  FUNC ## _getconstptr(const list_t *list, size_t idx) {\
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  return list->b + list->start + idx;                                          \
}                                                                              \
                                                                               \
static inline void    FUNC ## _set(list_t *list, size_t idx, obj_t obj) {      \
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  list->b[list->start + idx] = obj;                                            \
}                                                                              \
                                                                               \
static inline void    FUNC ## _alloc(list_t *list, size_t capacity) {          \
  list->capacity = capacity < 8 ? 8 : roundup2pow(capacity);                   \
  list->b = mc_alloc(list->capacity, sizeof(obj_t));                           \
  list->start = list->end = list->capacity / 2;                                \
}                                                                              \
                                                                               \
static inline void    FUNC ## _dealloc(list_t *list) {                         \
  madcrow_list_verify(list);                                                   \
  mc_free(list->b);                                                            \
  memset(list, 0, sizeof(list_t));                                             \
}                                                                              \
                                                                               \
static inline void    FUNC ## _capacity(list_t *list, size_t cap) {            \
  madcrow_list_verify(list);                                                   \
  if(cap > list->capacity) {                                                   \
    cap = roundup2pow(cap);                                                    \
    list->b = mc_realloc(list->b, list->capacity * sizeof(obj_t));             \
    list->capacity = cap;                                                      \
  }                                                                            \
}                                                                              \
                                                                               \
/* Add an element to the end of the list */                                    \
static inline size_t  FUNC ## _push(list_t *list, const obj_t *ptr, size_t n) {\
  madcrow_list_verify(list);                                                   \
  if(list->end + n > list->capacity) {                                         \
    size_t oldlen = FUNC ## _len(list), newlen = oldlen + n;                   \
    if(newlen >= list->capacity / 2) {                                         \
      list->capacity = list->start + newlen;                                   \
      list->capacity = roundup2pow(list->capacity);                            \
      list->b = mc_realloc(list->b, list->capacity * sizeof(obj_t));           \
    }                                                                          \
    else {                                                                     \
      size_t new_start = (list->capacity - newlen) / 2;                        \
      memmove(list->b+new_start, list->b+list->start, oldlen*sizeof(obj_t)); \
      list->start = new_start;                                                 \
      list->end = new_start + oldlen;                                          \
    }                                                                          \
  }                                                                            \
  assert(ptr);                                                                 \
  memcpy(list->b+list->end, ptr, n*sizeof(obj_t));                             \
  size_t idx = list->end - list->start;                                        \
  list->end += n;                                                              \
  return idx;                                                                  \
}                                                                              \
                                                                               \
/* Remove (and return) elements from the end of the list */                    \
/* @param ptr if != NULL, removed elements are copied to ptr */                \
static inline void    FUNC ## _pop(list_t *list, obj_t *ptr, size_t n) {       \
  madcrow_list_verify(list);                                                   \
  assert(list->start+n <= list->end);                                          \
  list->end -= n;                                                              \
  if(ptr) memcpy(ptr, list->b+list->end, n*sizeof(obj_t));                     \
}                                                                              \
                                                                               \
/* Add an element to the start of the list */                                  \
static inline size_t  FUNC ## _unshift(list_t *list, const obj_t *ptr, size_t n) {\
  madcrow_list_verify(list);                                                   \
  if(list->start == 0) {                                                       \
    size_t oldlen = FUNC ## _len(list), newlen = oldlen + n;                   \
    if(newlen >= list->capacity / 2) {                                         \
      list->capacity = roundup2pow(newlen);                                    \
      list->b = mc_realloc(list->b, list->capacity * sizeof(obj_t));           \
    }                                                                          \
    size_t new_start = (list->capacity - newlen) / 2 + n;                      \
    memmove(list->b+new_start, list->b, oldlen*sizeof(obj_t));                 \
    list->start = new_start;                                                   \
    list->end = new_start + oldlen;                                            \
  }                                                                            \
  assert(ptr);                                                                 \
  assert(list->start >= n);                                                    \
  list->start -= n;                                                            \
  memcpy(list->b+list->start, ptr, n*sizeof(obj_t));                           \
  return 0;                                                                    \
}                                                                              \
                                                                               \
/* Remove (and return) elements from the start of the list */                  \
/* @param ptr if != NULL, removed elements are copied to ptr */                \
static inline void    FUNC ## _shift(list_t *list, obj_t *ptr, size_t n) {     \
  madcrow_list_verify(list);                                                   \
  assert(list->start+n <= list->end);                                          \
  if(ptr) memcpy(ptr, list->b+list->start, n*sizeof(obj_t));                   \
  list->start += n;                                                            \
}                                                                              \
                                                                               \
static inline void    FUNC ## _reset(list_t *list) {                           \
  madcrow_list_verify(list);                                                   \
  list->start = list->end = list->capacity / 2;                                \
}                                                                              \
                                                                               \
static inline void    FUNC ## _copy(list_t *dst, const list_t *src) {          \
  madcrow_list_verify(src);                                                    \
  size_t len = FUNC ## _len(src);                                              \
  FUNC ## _capacity(dst, 2*len);                                               \
  dst->start = (dst->capacity - len) / 2;                                      \
  dst->end = dst->start + len;                                                 \
  memcpy(dst->b+dst->start, src->b+src->start, len * sizeof(obj_t));           \
  madcrow_list_verify(dst);                                                    \
}                                                                              \
                                                                               \
/* Add an element to the end of the list */                                    \
static inline size_t  FUNC ## _append(list_t *list, obj_t obj) {               \
  return FUNC ## _push(list, &obj, 1);                                         \
}                                                                              \
                                                                               \
static inline size_t  FUNC ## _prepend(list_t *list, obj_t obj) {              \
  return FUNC ## _unshift(list, &obj, 1);                                      \
}                                                                              \
                                                                               \
static inline obj_t   FUNC ## _lcut(list_t *list) {                            \
  madcrow_list_verify(list);                                                   \
  assert(list->start < list->end);                                             \
  return list->b[list->start++];                                               \
}                                                                              \
                                                                               \
static inline obj_t   FUNC ## _rcut(list_t *list) {                            \
  madcrow_list_verify(list);                                                   \
  assert(list->start < list->end);                                             \
  return list->b[--list->end];                                                 \
}                                                                              \
                                                                               \
static inline void    FUNC ## _getn(list_t *list, size_t idx,                  \
                                    obj_t *ptr, size_t n) {                    \
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  assert(list->start+idx+n <= list->end);                                      \
  memcpy(ptr, list->b+list->start+idx, n*sizeof(obj_t));                       \
}                                                                              \
                                                                               \
static inline void    FUNC ## _setn(list_t *list, size_t idx,                  \
                                    const obj_t *ptr, size_t n) {              \
  madcrow_list_verify(list);                                                   \
  assert(idx < list->end - list->start);                                       \
  assert(list->start+idx+n <= list->end);                                      \
  memcpy(list->b+list->start+idx, ptr, n*sizeof(obj_t));                       \
}                                                                              \

#endif /* MADCROW_LIST_H_ */
