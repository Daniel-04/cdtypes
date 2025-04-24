#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

/*
** ring buffer needs wrapping with negatives
*/
#define _MOD(a, b) (((a) % (typeof(a))(b) + (b)) % (b))

/*
** QUEUE_T(type) queue = {0};
*/
#define QUEUE_T(type)                                                          \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
    ssize_t start;                                                             \
  }

/*
** QUEUE_PUSH_BACK(queue, element);
*/
#define QUEUE_PUSH_BACK(queue, element)                                        \
  do {                                                                         \
    if ((queue).count >= (queue).capacity) {                                   \
      typeof(*(queue).buf) *_old_buf = (queue).buf;                            \
      typeof((queue).capacity) _old_capacity = (queue).capacity;               \
      (queue).capacity = (queue).capacity * 2 + 1;                             \
      (queue).buf = malloc((queue).capacity * sizeof(*(queue).buf));           \
      for (size_t i = 0; i < (queue).count; i++) {                             \
        (queue).buf[i] = _old_buf[_MOD((queue).start + i, _old_capacity)];     \
      }                                                                        \
      (queue).start = 0;                                                       \
      free(_old_buf);                                                          \
    }                                                                          \
    (queue).buf[_MOD((queue).start + (queue).count, (queue).capacity)] =       \
        (element);                                                             \
    (queue).count++;                                                           \
  } while (0)

/*
** QUEUE_PUSH_FRONT(queue, element);
*/
#define QUEUE_PUSH_FRONT(queue, element)                                       \
  do {                                                                         \
    if ((queue).count >= (queue).capacity) {                                   \
      typeof(*(queue).buf) *_old_buf = (queue).buf;                            \
      typeof((queue).capacity) _old_capacity = (queue).capacity;               \
      (queue).capacity = (queue).capacity * 2 + 1;                             \
      (queue).buf = malloc((queue).capacity * sizeof(*(queue).buf));           \
      for (size_t i = 0; i < (queue).count; i++) {                             \
        (queue).buf[i + 1] = _old_buf[_MOD((queue).start + i, _old_capacity)]; \
      }                                                                        \
      (queue).start = 1;                                                       \
      free(_old_buf);                                                          \
    }                                                                          \
    (queue).start = _MOD((queue).start - 1, (queue).capacity);                 \
    (queue).buf[(queue).start] = (element);                                    \
    (queue).count++;                                                           \
  } while (0)

#define _GET_QUEUE_PEEK_FRONT(_1, _2, NAME, ...) NAME
#define _QUEUE_PEEK_FRONT_DEFAULT(queue, default)                              \
  ((queue).count > 0 ? (queue).buf[(queue).start] : (default))
#define _QUEUE_PEEK_FRONT_ZERO(queue)                                          \
  _QUEUE_PEEK_FRONT_DEFAULT(queue, (typeof(*(queue).buf))0)

/*
** QUEUE_PEEK_FRONT(queue);
** QUEUE_PEEK_FRONT(queue, default);
*/
#define QUEUE_PEEK_FRONT(...)                                                  \
  _GET_QUEUE_PEEK_FRONT(__VA_ARGS__, _QUEUE_PEEK_FRONT_DEFAULT,                \
                        _QUEUE_PEEK_FRONT_ZERO)(__VA_ARGS__)

#define _GET_QUEUE_PEEK_BACK(_1, _2, NAME, ...) NAME
#define _QUEUE_PEEK_BACK_DEFAULT(queue, default)                               \
  ((queue).count > 0                                                           \
       ? (queue)                                                               \
             .buf[_MOD((queue).start + (queue).count - 1, (queue).capacity)]   \
       : (default))
#define _QUEUE_PEEK_BACK_ZERO(queue)                                           \
  _QUEUE_PEEK_BACK_DEFAULT(queue, (typeof(*(queue).buf))0)

/*
** QUEUE_PEEK_BACK(queue);
** QUEUE_PEEK_BACK(queue, default);
*/
#define QUEUE_PEEK_BACK(...)                                                   \
  _GET_QUEUE_PEEK_BACK(__VA_ARGS__, _QUEUE_PEEK_BACK_DEFAULT,                  \
                       _QUEUE_PEEK_BACK_ZERO)(__VA_ARGS__)

#define _GET_QUEUE_POP_BACK(_1, _2, NAME, ...) NAME
#define _QUEUE_POP_BACK_DEFAULT(queue, default)                                \
  ((queue).count > 0                                                           \
       ? (queue).buf[_MOD((queue).start + --(queue).count, queue.capacity)]    \
       : default)
#define _QUEUE_POP_BACK_ZERO(queue)                                            \
  _QUEUE_POP_BACK_DEFAULT(queue, (typeof(*(queue).buf))0)

/*
** QUEUE_POP_BACK(queue);
** QUEUE_POP_BACK(queue, default);
*/
#define QUEUE_POP_BACK(...)                                                    \
  _GET_QUEUE_POP_BACK(__VA_ARGS__, _QUEUE_POP_BACK_DEFAULT,                    \
                      _QUEUE_POP_BACK_ZERO)(__VA_ARGS__)

#define _GET_QUEUE_POP_FRONT(_1, _2, NAME, ...) NAME
#define _QUEUE_POP_FRONT_DEFAULT(queue, default)                               \
  ({                                                                           \
    typeof(*(queue).buf) _result;                                              \
    if ((queue).count > 0) {                                                   \
      typeof((queue).start) _old_start = (queue).start;                        \
      (queue).start = _MOD((queue).start + 1, (queue).capacity);               \
      (queue).count--;                                                         \
      _result = (queue).buf[_old_start];                                       \
    } else {                                                                   \
      _result = (default);                                                     \
    }                                                                          \
    _result;                                                                   \
  })
#define _QUEUE_POP_FRONT_ZERO(queue)                                           \
  _QUEUE_POP_FRONT_DEFAULT(queue, (typeof(*(queue).buf))0)

/*
** QUEUE_POP_FRONT(queue);
** QUEUE_POP_FRONT(queue, default);
*/
#define QUEUE_POP_FRONT(...)                                                   \
  _GET_QUEUE_POP_FRONT(__VA_ARGS__, _QUEUE_POP_FRONT_DEFAULT,                  \
                       _QUEUE_POP_FRONT_ZERO)(__VA_ARGS__)

/*
** QUEUE_FREE(queue);
** SAFETY: double free if used after queue creation without element insertion
*/
#define QUEUE_FREE(queue)                                                      \
  do {                                                                         \
    free((queue).buf);                                                         \
    (queue).buf = NULL;                                                        \
    (queue).count = 0;                                                         \
    (queue).capacity = 0;                                                      \
    (queue).start = 0;                                                         \
  } while (0)

/*
** QUEUE_DBG(queue, format)
*/
#define QUEUE_DBG(queue, fmt)                                                  \
  do {                                                                         \
    printf("Queue (count=%zu, capacity=%zu): [", (queue).count,                \
           (queue).capacity);                                                  \
    for (size_t i = 0; i < (queue).count; i++) {                               \
      printf(fmt, (queue).buf[_MOD((queue).start + i, (queue).capacity)]);     \
      if (i < (queue).count - 1) {                                             \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // QUEUE_H_
