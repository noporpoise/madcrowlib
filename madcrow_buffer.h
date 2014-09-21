#ifndef OBJ_MACRO_BUFFER_H_
#define OBJ_MACRO_BUFFER_H_

#include <stdlib.h>
#include <string.h> // memset
#include <assert.h>

//
// madcrow_buffer.h
// Define a buffer with functions to alloc, resize, add, append, reset etc.
//
// Example:
//
//   #include "madcrow_buffer.h"
//   madcrow_buffer_create(charbuf,String,char)
//
// Creates:
//
//   typedef struct {
//     char *data;
//     size_t len, capacity;
//   } String;
//
//   void    charbuf_alloc       (String *buf, size_t capacity)
//   void    charbuf_dealloc     (String *buf)
//   void    charbuf_reset       (String *buf)
//   void    charbuf_capacity    (String *buf, size_t capacity)
//   size_t  charbuf_add         (String *buf, char obj)
//   ssize_t charbuf_attempt_add (String *buf, char obj)
//   void    charbuf_append      (String *buf, char *obj, size_t n)
//   void    charbuf_shift_left  (String *buf, size_t n)
//   void    charbuf_shift_right (String *buf, size_t n)
//
//  String string = madcrow_buffer_init;
//  madcrow_buffer_verify(&string);
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


#define madcrow_buffer_init {.data = NULL, .len = 0, .capacity = 0}

#define madcrow_buffer_verify(buf) do {                                        \
  assert(buf->len <= buf->capacity);                                           \
  assert(buf->capacity == 0 || buf->data != NULL);                             \
} while(0)

#define madcrow_buffer_create(FUNC,buf_t,obj_t)                                \
                                                                               \
typedef struct {                                                               \
  obj_t *data;                                                                 \
  size_t len, capacity;                                                        \
} buf_t;                                                                       \
                                                                               \
/* Define functions with unused attribute in case they're not used */          \
static inline void FUNC ## _alloc(buf_t *buf, size_t capacity)                 \
 __attribute__((unused));                                                      \
static inline void FUNC ## _dealloc(buf_t *buf)                                \
 __attribute__((unused));                                                      \
static inline void FUNC ## _capacity(buf_t *buf, size_t cap)                   \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _add(buf_t *buf, obj_t obj)                       \
 __attribute__((unused));                                                      \
static inline ssize_t FUNC ## _attempt_add(buf_t *buf, obj_t obj)              \
 __attribute__((unused));                                                      \
static inline size_t FUNC ## _append(buf_t *buf, const obj_t *obj, size_t n)   \
 __attribute__((unused));                                                      \
static inline void FUNC ## _reset(buf_t *buf)                                  \
 __attribute__((unused));                                                      \
static inline void FUNC ## _shift_left(buf_t *buf, size_t nel)                 \
 __attribute__((unused));                                                      \
static inline void FUNC ## _shift_right(buf_t *buf, size_t nel)                \
 __attribute__((unused));                                                      \
                                                                               \
                                                                               \
static inline void FUNC ## _alloc(buf_t *buf, size_t capacity) {               \
  buf->capacity = capacity;                                                    \
  buf->data = MC_MALLOC(buf->capacity * sizeof(obj_t));                        \
  buf->len = 0;                                                                \
}                                                                              \
                                                                               \
static inline void FUNC ## _dealloc(buf_t *buf) {                              \
  free(buf->data);                                                             \
  memset(buf, 0, sizeof(buf_t));                                               \
}                                                                              \
                                                                               \
static inline void FUNC ## _capacity(buf_t *buf, size_t cap) {                 \
  if(cap > buf->capacity) {                                                    \
    cap = roundup2pow(cap);                                                    \
    buf->data = MC_REALLOC(buf->data, buf->capacity * sizeof(obj_t));          \
    buf->capacity = cap;                                                       \
  }                                                                            \
}                                                                              \
                                                                               \
/* Returns index of new object in buffer */                                    \
static inline size_t FUNC ## _add(buf_t *buf, obj_t obj) {                     \
  FUNC ## _capacity(buf, buf->len+1);                                          \
  memcpy(buf->data+buf->len, &obj, sizeof(obj));                               \
  return buf->len++;                                                           \
}                                                                              \
                                                                               \
/* Returns index or -1 on failure */                                           \
static inline ssize_t FUNC ## _attempt_add(buf_t *buf, obj_t obj) {            \
  if(buf->len >= buf->capacity) return -1;                                     \
  memcpy(buf->data+buf->len, &obj, sizeof(obj));                               \
  return buf->len++;                                                           \
}                                                                              \
                                                                               \
/* Returns index of first new object in buffer */                              \
static inline size_t FUNC ## _append(buf_t *buf, const obj_t *obj, size_t n) { \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memcpy(buf->data+buf->len, obj, n*sizeof(obj_t));                            \
  size_t idx = buf->len;                                                       \
  buf->len += n;                                                               \
  return idx;                                                                  \
}                                                                              \
                                                                               \
static inline void FUNC ## _shift_left(buf_t *buf, size_t n)                   \
{                                                                              \
  FUNC ## _capacity(buf, buf->len+n);                                          \
  memmove(buf->data+n, buf->data, buf->len * sizeof(obj_t));                   \
  buf->len += n;                                                               \
}                                                                              \
                                                                               \
static inline void FUNC ## _shift_right(buf_t *buf, size_t n)                  \
{                                                                              \
  assert(n <= buf->len);                                                       \
  memmove(buf->data, buf->data+n, (buf->len - n)*sizeof(obj_t));               \
  buf->len -= n;                                                               \
}                                                                              \
                                                                               \
static inline void FUNC ## _reset(buf_t *buf) { buf->len = 0; }                \
\


#endif /* OBJ_MACRO_BUFFER_H_ */
