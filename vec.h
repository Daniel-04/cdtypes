#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>

/*
** vec_t(type) vector = {0};
*/
#define vec_t(type)                                                            \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
  }

/*
** vec_append(vector, element);
*/
#define vec_append(vec, element)                                               \
  do {                                                                         \
    if ((vec)->count >= (vec)->capacity) {                                     \
      (vec)->capacity = (vec)->capacity * 2 + 1;                               \
      (vec)->buf = realloc((vec)->buf, (vec)->capacity * sizeof(element));     \
    }                                                                          \
    (vec)->buf[(vec)->count] = (element);                                      \
    (vec)->count++;                                                            \
  } while (0)

#define _get_vec_pop(_1, _2, NAME, ...) NAME
#define _vec_pop_default(vec, default)                                         \
  ((vec)->count > 0 ? (vec)->buf[--(vec)->count] : default)
#define _vec_pop_zero(vec)                                                     \
  ((vec)->count > 0 ? (vec)->buf[--(vec)->count] : (typeof(*(vec)->buf))0)
/*
** vec_pop(&vector);
** vec_pop(&vector, default);
*/
#define vec_pop(...)                                                           \
  _get_vec_pop(__VA_ARGS__, _vec_pop_default, _vec_pop_zero)(__VA_ARGS__)

/*
** vec_free(&vector);
*/
#define vec_free(vec)                                                          \
  do {                                                                         \
    free((vec)->buf);                                                          \
    (vec)->buf = NULL;                                                         \
    (vec)->count = 0;                                                          \
    (vec)->capacity = 0;                                                       \
  } while (0)

/*
** vec_dbg(&vector, format)
*/
#define vec_dbg(vec, fmt)                                                      \
  do {                                                                         \
    putchar('[');                                                              \
    for (size_t i = 0; i < (vec)->count; i++) {                                \
      printf(fmt, (vec)->buf[i]);                                              \
      if (i < (vec)->count - 1) {                                              \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // VEC_H_
