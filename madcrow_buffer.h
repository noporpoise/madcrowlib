#ifndef MADCROW_BUFFER_H_
#define MADCROW_BUFFER_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>
#include <unistd.h> // ssize_t

//
// madcrow_buffer.h
// Define a buffer with functions to alloc, resize, add, append, reset etc.
//
// Example:
//
//   #include "madcrow_buffer.h"
//   madcrow_buffer(charbuf,String,char)
//
// Creates:
//
//   typedef struct {
//     char *b;
//     size_t len, size;
//   } String;
//
//   String* charbuf_new         (size_t capacity)
//   void    charbuf_destroy     (String *buf)
//   void    charbuf_alloc       (String *buf, size_t capacity)
//   void    charbuf_dealloc     (String *buf)
//   void    charbuf_reset       (String *buf)
//   void    charbuf_capacity    (String *buf, size_t capacity)
//   void    charbuf_len         (const String *buf)
//
// Pass object:
//   size_t  charbuf_add         (String *buf, char obj)
//   char    charbuf_remove      (String *buf)
//   char    charbuf_get         (String *buf, size_t idx)
//   void    charbuf_set         (String *buf, size_t idx, char obj)
//
// Pass pointers:
//   void    charbuf_getn        (String *buf, size_t idx, char *ptr, size_t n)
//   void    charbuf_setn        (String *buf, size_t idx,
//                                char const *ptr, size_t n)
//   char*   charbuf_getptr      (CharList *list, size_t idx)
//
//   size_t  charbuf_push        (String *buf, char const *ptr, size_t n)
//   void    charbuf_pop         (String *buf, char *ptr, size_t n)
//   void    charbuf_unshift     (String *buf, char const *ptr, size_t n)
//   void    charbuf_shift       (String *buf, char *ptr, size_t n)
//
//   void    charbuf_copy        (String *dst, const String *src)
//   void    charbuf_resize      (String *buf, size_t n)
//
// There need to be removed:
//   ssize_t charbuf_push_try    (String *buf, char const *ptr, size_t n)
//   size_t  charbuf_push_rpt    (String *buf, char const *obj, size_t n)
//   size_t  charbuf_push_zero   (String *buf, size_t n)
//   ssize_t charbuf_unshift_try (String *buf, char const *ptr, size_t n)
//   void    charbuf_unshift_rpt (String *buf, char const *obj, size_t n)
//   void    charbuf_unshift_zero(String *buf, size_t n)
//
//  String string;
//  char_buf_alloc(&string, 1024);
//  madcrow_buffer_verify(&string);
//

// Round a number up to the nearest number that is a power of two
#ifndef leading_zeros
  #define leading_zeros(x) ((x) ? (__typeof(x))__builtin_clzll(x) \
                                : (__typeof(x))sizeof(x)*8)
#endif

#ifndef roundup2pow
  #define roundup2pow(x) (1UL << (64 - leading_zeros((uint64_t)(x))))
#endif

#define madcrow_buffer_init {.b = NULL, .len = 0, .capacity = 0}

#define madcrow_buffer_verify(buf) do {                                        \
  assert(buf->len <= buf->size);                                               \
  assert(buf->size == 0 || buf->b != NULL);                                    \
} while(0)

#define MC_INIT_MEM_WIPE(arr,n) memset(arr, 0, (n)*sizeof(*(arr)))
#define MC_INIT_MEM_UNDEF(arr,n)

#define madcrow_buffer(FUNC,buf_t,obj_t) \
        madcrow_buffer2(FUNC,buf_t,obj_t,calloc,realloc,free,MC_INIT_MEM_UNDEF)

#define madcrow_buffer_wipe(FUNC,buf_t,obj_t) \
        madcrow_buffer2(FUNC,buf_t,obj_t,calloc,realloc,free,MC_INIT_MEM_WIPE)

// init_mem_f is one of MC_INIT_MEM_WIPE or MC_INIT_MEM_UNDEF
#define madcrow_buffer2(FUNC,buf_t,obj_t,mc_alloc,mc_realloc,mc_free,init_mem_f)\
                                                                               \
typedef struct {                                                               \
  obj_t *b;                                                                    \
  size_t len, size;                                                            \
} buf_t;                                                                       \
                                                                               \
/* Define functions with unused attribute in case they're not used */          \
static inline buf_t*  FUNC ## _new(size_t capacity)                            \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _destroy(buf_t *buf)                             \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _alloc(buf_t *buf, size_t capacity)              \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _dealloc(buf_t *buf)                             \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _reset(buf_t *buf)                               \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _capacity(buf_t *buf, size_t cap)                \
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _len(const buf_t *buf)                           \
 __attribute__((unused));                                                      \
\
static inline size_t  FUNC ## _add(buf_t *buf, obj_t obj)                      \
 __attribute__((unused));                                                      \
static inline obj_t   FUNC ## _remove(buf_t *buf)                              \
 __attribute__((unused));                                                      \
static inline obj_t   FUNC ## _get(buf_t *buf, size_t idx)                     \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _set(buf_t *buf, size_t idx, obj_t obj)          \
 __attribute__((unused));                                                      \
\
static inline void    FUNC ## _getn(buf_t *buf, size_t idx, obj_t *ptr, size_t n)\
 __attribute__((unused));                                                      \
static inline void    FUNC ## _setn(buf_t *buf, size_t idx,                    \
                                    obj_t const *ptr, size_t n)                \
 __attribute__((unused));                                                      \
static inline obj_t*  FUNC ## _getptr(buf_t *buf, size_t idx)                  \
 __attribute__((unused));                                                      \
\
static inline size_t  FUNC ## _push(buf_t *buf, obj_t const *ptr, size_t n)    \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _pop(buf_t *buf, obj_t *ptr, size_t n)           \
 __attribute__((unused));                                                      \
\
static inline void    FUNC ## _unshift(buf_t *buf, obj_t const *ptr, size_t n) \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _shift(buf_t *buf, obj_t *ptr, size_t n)         \
 __attribute__((unused));                                                      \
\
/* Potentially removing these: */ \
static inline ssize_t FUNC ## _push_try(buf_t *buf, obj_t const *ptr, size_t n)\
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _push_rpt(buf_t *buf, obj_t const *obj, size_t n)\
 __attribute__((unused));                                                      \
static inline ssize_t FUNC ## _unshift_try(buf_t *buf, obj_t const *ptr, size_t n)\
 __attribute__((unused));                                                      \
static inline void    FUNC ## _unshift_rpt(buf_t *buf, obj_t const *obj, size_t n)\
 __attribute__((unused));                                                      \
static inline size_t  FUNC ## _push_zero(buf_t *buf, size_t n)                 \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _unshift_zero(buf_t *buf, size_t n)              \
 __attribute__((unused));                                                      \
\
static inline void    FUNC ## _copy(buf_t *dst, const buf_t *src)              \
 __attribute__((unused));                                                      \
static inline void    FUNC ## _resize(buf_t *buf, size_t len)                  \
 __attribute__((unused));                                                      \
                                                                               \
static inline buf_t*  FUNC ## _new(size_t capacity)                            \
{                                                                              \
  buf_t *buf = mc_alloc(1, sizeof(buf_t));                                     \
  if(buf) FUNC ## _alloc(buf, capacity);                                       \
  return buf;                                                                  \
}                                                                              \
                                                                               \
static inline void    FUNC ## _destroy(buf_t *buf)                             \
{                                                                              \
  FUNC ## _dealloc(buf);                                                       \
  mc_free(buf);                                                                \
}                                                                              \
                                                                               \
static inline void    FUNC ## _alloc(buf_t *buf, size_t capacity) {            \
  buf->size = capacity;                                                        \
  buf->b = mc_alloc(buf->size, sizeof(obj_t));                                 \
  buf->len = 0;                                                                \
}                                                                              \
                                                                               \
static inline void    FUNC ## _dealloc(buf_t *buf) {                           \
  mc_free(buf->b);                                                             \
  memset(buf, 0, sizeof(buf_t));                                               \
}                                                                              \
                                                                               \
static inline void    FUNC ## _reset(buf_t *buf) {                             \
  init_mem_f(buf->b, buf->len);                                                \
  buf->len = 0;                                                                \
}                                                                              \
                                                                               \
static inline void    FUNC ## _capacity(buf_t *buf, size_t cap) {              \
  if(cap > buf->size) {                                                        \
    cap = roundup2pow(cap);                                                    \
    buf->b = mc_realloc(buf->b, cap * sizeof(obj_t));                          \
    init_mem_f(buf->b + buf->size, cap - buf->size);                           \
    buf->size = cap;                                                           \
  }                                                                            \
}                                                                              \
                                                                               \
static inline size_t  FUNC ## _len(const buf_t *buf) {                         \
  return buf->len;                                                             \
}                                                                              \
                                                                               \
/*                                                   */                        \
/* Functions that pass/return objects NOT pointers   */                        \
/*                                                   */                        \
                                                                               \
/* Add an object to the end of the buffer */                                   \
/* Returns index of new object in buffer */                                    \
static inline size_t  FUNC ## _add(buf_t *buf, obj_t obj) {                    \
  FUNC ## _capacity(buf, buf->len+1);                                          \
  memcpy(buf->b+buf->len, &obj, sizeof(obj));                                  \
  return buf->len++;                                                           \
}                                                                              \
                                                                               \
/* Remove an object from the end of the buffer and return it */                \
static inline obj_t   FUNC ## _remove(buf_t *buf)                              \
{                                                                              \
  assert(buf->len > 0);                                                        \
  obj_t tmp = buf->b[--buf->len];                                              \
  init_mem_f(buf->b+buf->len, 1);                                              \
  return tmp;                                                                  \
}                                                                              \
                                                                               \
/* Fetch a given element */                                                    \
static inline obj_t   FUNC ## _get(buf_t *buf, size_t idx)                     \
{                                                                              \
  assert(idx < buf->len);                                                      \
  return buf->b[idx];                                                          \
}                                                                              \
                                                                               \
/* Set a given element */                                                      \
static inline void    FUNC ## _set(buf_t *buf, size_t idx, obj_t obj)          \
{                                                                              \
  assert(idx < buf->len);                                                      \
  memmove(&buf->b[idx], &obj, sizeof(obj_t));                                  \
}                                                                              \
                                                                               \
/*                                                   */                        \
/* Functions that pass/return pointers NOT objects   */                        \
/*                                                   */                        \
                                                                               \
/* Fetch a given element */                                                    \
static inline void    FUNC ## _getn(buf_t *buf, size_t idx, obj_t *ptr, size_t n)\
{                                                                              \
  assert(idx+n <= buf->len);                                                   \
  memmove(ptr, buf->b+idx, n * sizeof(obj_t));                                 \
}                                                                              \
                                                                               \
/* Set a given element */                                                      \
static inline void    FUNC ## _setn(buf_t *buf, size_t idx,                    \
                                    obj_t const *ptr, size_t n)                \
{                                                                              \
  assert(idx+n <= buf->len);                                                   \
  memmove(buf->b+idx, ptr, n * sizeof(obj_t));                                 \
}                                                                              \
                                                                               \
static inline obj_t*  FUNC ## _getptr(buf_t *buf, size_t idx) {                \
  assert(idx < buf->len);                                                      \
  return buf->b + idx;                                                         \
}                                                                              \
                                                                               \
                                                                               \
/* Append n objects to the end of the buffer */                                \
static inline size_t  FUNC ## _push(buf_t *buf, obj_t const *ptr, size_t n)    \
{                                                                              \
  size_t idx = buf->len;                                                       \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memmove(buf->b+buf->len, ptr, n * sizeof(obj_t));                            \
  buf->len += n;                                                               \
  return idx;                                                                  \
}                                                                              \
                                                                               \
/* Remove and return last n elements */                                        \
/* @param ptr if != NULL, removed elements are copied to ptr */                \
static inline void    FUNC ## _pop(buf_t *buf, obj_t *ptr, size_t n)           \
{                                                                              \
  assert(buf->len >= n);                                                       \
  buf->len -= n;                                                               \
  if(ptr) memmove(ptr, buf->b+buf->len, n * sizeof(obj_t));                    \
  init_mem_f(buf->b+buf->len, n);                                              \
}                                                                              \
                                                                               \
/* Add items to the start of a buffer */                                       \
static inline void    FUNC ## _unshift(buf_t *buf, obj_t const *ptr, size_t n) \
{                                                                              \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memmove(buf->b+n, buf->b, buf->len * sizeof(obj_t));                         \
  memmove(buf->b, ptr, n * sizeof(obj_t));                                     \
  buf->len += n;                                                               \
}                                                                              \
                                                                               \
/* Remove and return items to the start of a buffer */                         \
/* @param ptr if != NULL, removed elements are copied to ptr */                \
static inline void    FUNC ## _shift(buf_t *buf, obj_t *ptr, size_t n)         \
{                                                                              \
  assert(n <= buf->len);                                                       \
  buf->len -= n;                                                               \
  if(ptr) memmove(ptr, buf->b, n * sizeof(obj_t));                             \
  memmove(buf->b, buf->b+n, buf->len * sizeof(obj_t));                         \
  init_mem_f(buf->b+buf->len, n);                                              \
}                                                                              \
                                                                               \
/* Returns index or -1 on failure */                                           \
static inline ssize_t FUNC ## _push_try(buf_t *buf, obj_t const *ptr, size_t n)\
{                                                                              \
  if(buf->len + n > buf->size) return -1;                                      \
  memmove(buf->b+buf->len, ptr, n*sizeof(obj_t));                              \
  ssize_t idx = buf->len; buf->len += n;                                       \
  return idx;                                                                  \
}                                                                              \
                                                                               \
/* Add the same item to the end of the array n times */                        \
/* Returns the index of the first item added */                                \
static inline size_t  FUNC ## _push_rpt(buf_t *buf, obj_t const *obj, size_t n)\
{                                                                              \
  size_t idx = buf->len, end = buf->len+n;                                     \
  FUNC ## _capacity(buf, end);                                                 \
  for(; buf->len < end; buf->len++)                                            \
    memmove(buf->b+buf->len, obj, sizeof(obj_t));                              \
  return idx;                                                                  \
}                                                                              \
                                                                               \
/* Attempt to add to the start of the array without reallocating */            \
/* Return -1 if failed, 0 on success */                                        \
static inline ssize_t FUNC ## _unshift_try(buf_t *buf, obj_t const *ptr, size_t n)\
{                                                                              \
  if(buf->len + n > buf->size) return -1;                                      \
  memmove(buf->b+n, buf->b, buf->len * sizeof(obj_t));                         \
  memmove(buf->b,   ptr,    n * sizeof(obj_t));                                \
  buf->len += n;                                                               \
  return 0;                                                                    \
}                                                                              \
                                                                               \
/* Add the same item to the start of the array n times */                      \
static inline void    FUNC ## _unshift_rpt(buf_t *buf, obj_t const *obj, size_t n)\
{                                                                              \
  size_t i;                                                                    \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memmove(buf->b+n, buf->b, buf->len*sizeof(obj_t));                           \
  for(i = 0; i < n; i++) memmove(buf->b+i, obj, sizeof(obj_t));                \
  buf->len += n;                                                               \
}                                                                              \
                                                                               \
/* Add a zero'd item to the end of the array n times */                        \
/* Returns the index of the first item added */                                \
static inline size_t  FUNC ## _push_zero(buf_t *buf, size_t n)                 \
{                                                                              \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memset(buf->b+buf->len, 0, n*sizeof(obj_t));                                 \
  size_t idx = buf->len; buf->len += n;                                        \
  return idx;                                                                  \
}                                                                              \
                                                                               \
/* Add a zero'd item to the start of the array n times */                      \
static inline void    FUNC ## _unshift_zero(buf_t *buf, size_t n)              \
{                                                                              \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memmove(buf->b+n, buf->b, buf->len*sizeof(obj_t));                           \
  memset(buf->b, 0, n*sizeof(obj_t));                                          \
  buf->len += n;                                                               \
}                                                                              \
                                                                               \
                                                                               \
static inline void    FUNC ## _copy(buf_t *dst, const buf_t *src) {            \
  FUNC ## _capacity(dst, src->len);                                            \
  memmove(dst->b, src->b, sizeof(obj_t) * src->len);                           \
  dst->len = src->len;                                                         \
}                                                                              \
                                                                               \
static inline void    FUNC ## _resize(buf_t *buf, size_t len) {                \
  FUNC ## _capacity(buf, len);                                                 \
  if(buf->len < len) buf->len = len;                                           \
}                                                                              \

#endif /* MADCROW_BUFFER_H_ */
