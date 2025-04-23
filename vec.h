#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>

/*
** VEC_T(type) vector = {0};
*/
#define VEC_T(type)                                                            \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
  }

/*
** VEC_PUSH(vector, element);
*/
#define VEC_PUSH(vec, element)                                                 \
  do {                                                                         \
    if ((vec).count >= (vec).capacity) {                                       \
      (vec).capacity = (vec).capacity * 2 + 1;                                 \
      (vec).buf = realloc((vec).buf, (vec).capacity * sizeof(*(vec).buf));     \
    }                                                                          \
    (vec).buf[(vec).count] = (element);                                        \
    (vec).count++;                                                             \
  } while (0)

#define _GET_VEC_POP(_1, _2, NAME, ...) NAME
#define _VEC_POP_DEFAULT(vec, default)                                         \
  ((vec).count > 0 ? (vec).buf[--(vec).count] : default)
#define _VEC_POP_ZERO(vec) _VEC_POP_DEFAULT(vec, (typeof(*(vec).buf))0)

/*
** VEC_POP(vector);
** VEC_POP(vector, default);
*/
#define VEC_POP(...)                                                           \
  _GET_VEC_POP(__VA_ARGS__, _VEC_POP_DEFAULT, _VEC_POP_ZERO)(__VA_ARGS__)

/*
** VEC_FREE(vector);
** SAFETY: double free if used after vector creation without element insertion
*/
#define VEC_FREE(vec)                                                          \
  do {                                                                         \
    free((vec).buf);                                                           \
    (vec).buf = NULL;                                                          \
    (vec).count = 0;                                                           \
    (vec).capacity = 0;                                                        \
  } while (0)

/*
** VEC_DBG(vector, format)
*/
#define VEC_DBG(vec, fmt)                                                      \
  do {                                                                         \
    printf("Vector (count=%zu, capacity=%zu): [", (vec).count,                 \
           (vec).capacity);                                                    \
    for (size_t i = 0; i < (vec).count; i++) {                                 \
      printf(fmt, (vec).buf[i]);                                               \
      if (i < (vec).count - 1) {                                               \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // VEC_H_
