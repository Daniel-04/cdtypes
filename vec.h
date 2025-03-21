#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>

#define vec_t(type)                                                            \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
  }

#define vec_append(vec, element)                                               \
  do {                                                                         \
    if ((vec)->count >= (vec)->capacity) {                                     \
      (vec)->capacity = (vec)->capacity * 2 + 1;                               \
      (vec)->buf = realloc((vec)->buf, (vec)->capacity * sizeof(element))      \
    }                                                                          \
    (vec)->buf[(vec)->count] = (element);                                      \
    (vec)->count++;                                                            \
  } while (0)

#define vec_pop(vec) ((vec)->count > 0 ? (vec)->buf[--(vec)->count] : NULL)

#define vec_free(vec)                                                          \
  do {                                                                         \
    free((vec)->buf);                                                          \
    (vec)->buf = NULL;                                                         \
    (vec)->count = 0;                                                          \
    (vec)->capacity = 0;                                                       \
  } while (0)

#endif // VEC_H_
