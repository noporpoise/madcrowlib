#ifndef OBJ_MACRO_LIST_H_
#define OBJ_MACRO_LIST_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>

// Round a number up to the nearest number that is a power of two
#ifndef leading_zeros
  #define leading_zeros(x) ((x) ? (__typeof(x))__builtin_clzll(x) : (__typeof(x))sizeof(x)*8)
#endif
#ifndef roundup2pow
  #define roundup2pow(x) (1UL << (64 - leading_zeros(x)))
#endif

#define obj_macro_list_init {.data = NULL, .start = 0, .end = 0, .capacity = 0}

#define obj_macro_list_verify(list) do {                                       \
  assert(list->start <= list->end);                                            \
  assert(list->end <= list->capacity);                                         \
  assert(list->capacity == 0 || list->data != NULL);                           \
} while(0)

#define obj_macro_list_create(FUNC,list_t,obj_t)                               \
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
static inline void FUNC ## _ensure_capacity(list_t *list, size_t cap)          \
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
static inline size_t FUNC ## _length(list_t *list)                             \
 __attribute__((unused));                                                      \
static inline obj_t* FUNC ## _get(list_t *list, size_t idx)                    \
 __attribute__((unused));                                                      \
                                                                               \
static inline size_t FUNC ## _length(list_t *list) {                           \
  obj_macro_list_verify(list);                                                 \
  return list->end - list->start + 1;                                          \
}                                                                              \
                                                                               \
static inline obj_t* FUNC ## _get(list_t *list, size_t idx) {                  \
  assert(idx < list->end);                                                     \
  obj_macro_list_verify(list);                                                 \
  return list->data + list->start + idx;                                       \
}                                                                              \
                                                                               \
static inline void FUNC ## _alloc(list_t *list, size_t capacity) {             \
  list->capacity = capacity < 8 ? 8 : roundup2pow(capacity);                   \
  list->data = malloc(list->capacity * sizeof(obj_t));                         \
  list->start = list->end = list->capacity / 2;                                \
}                                                                              \
                                                                               \
static inline void FUNC ## _dealloc(list_t *list) {                            \
  obj_macro_list_verify(list);                                                 \
  free(list->data);                                                            \
  memset(list, 0, sizeof(list_t));                                             \
}                                                                              \
                                                                               \
static inline void FUNC ## _ensure_capacity(list_t *list, size_t cap) {        \
  if(cap > list->capacity) {                                                   \
    cap = roundup2pow(cap);                                                    \
    list->data = realloc(list->data, list->capacity * sizeof(obj_t));          \
    list->capacity = cap;                                                      \
  }                                                                            \
}                                                                              \
                                                                               \
/* Add an element to the end of the list */                                    \
static inline size_t FUNC ## _push(list_t *list, obj_t obj) {                  \
  obj_macro_list_verify(list);                                                 \
  if(list->end >= list->capacity) {                                            \
    size_t n = FUNC ## _length(list);                                          \
    size_t half = list->capacity / 2;                                          \
    if(n >= half) {                                                            \
      list->capacity *= 2;                                                     \
      list->data = realloc(list->data, list->capacity);                        \
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
  obj_macro_list_verify(list);                                                 \
  return list->data[--list->end];                                              \
}                                                                              \
                                                                               \
/* Add an element to the start of the list */                                  \
static inline size_t FUNC ## _shift(list_t *list, obj_t obj) {                 \
  obj_macro_list_verify(list);                                                 \
  if(list->start == 0) {                                                       \
    size_t n = FUNC ## _length(list);                                          \
    size_t half = list->capacity / 2;                                          \
    if(n >= half) {                                                            \
      list->capacity *= 2;                                                     \
      list->data = realloc(list->data, list->capacity);                        \
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
  obj_macro_list_verify(list);                                                 \
  return list->data[list->start++];                                            \
}                                                                              \
                                                                               \
static inline void FUNC ## _reset(list_t *list) {                              \
  obj_macro_list_verify(list);                                                 \
  list->start = list->end = list->capacity / 2;                                \
}

#endif /* OBJ_MACRO_LIST_H_ */
