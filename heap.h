#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>

/*
** default min heap
*/
#define _HEAP_DEFAULT_COMPARE(a, b) ((a) < (b))

/*
** HEAP_T(type) heap = {0}
** HEAP_T(type) heap = {.cmp = c_function}
*/
#define HEAP_T(type)                                                           \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
    int (*cmp)(type a, type b);                                                \
  }

#define _HEAP_DO_COMPARE(heap, a, b)                                           \
  ((heap).cmp ? (heap).cmp(a, b) : _HEAP_DEFAULT_COMPARE(a, b))

#define _HEAP_SIFT_UP(heap, start_index)                                       \
  do {                                                                         \
    size_t _child_idx = (start_index);                                         \
    while (_child_idx > 0) {                                                   \
      size_t _parent_idx = (_child_idx - 1) / 2;                               \
      if (_HEAP_DO_COMPARE(heap, (heap).buf[_child_idx],                       \
                           (heap).buf[_parent_idx])) {                         \
        typeof(*(heap).buf) _tmp = (heap).buf[_child_idx];                     \
        (heap).buf[_child_idx] = (heap).buf[_parent_idx];                      \
        (heap).buf[_parent_idx] = _tmp;                                        \
        _child_idx = _parent_idx;                                              \
      } else {                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define _HEAP_SIFT_DOWN(heap)                                                  \
  do {                                                                         \
    if ((heap).count <= 1)                                                     \
      break;                                                                   \
    size_t _parent_idx = 0;                                                    \
    size_t _n = (heap).count;                                                  \
    while (1) {                                                                \
      size_t _left_child_idx = 2 * _parent_idx + 1;                            \
      size_t _right_child_idx = 2 * _parent_idx + 2;                           \
      size_t _priority_idx = _parent_idx;                                      \
      if (_left_child_idx < _n &&                                              \
          _HEAP_DO_COMPARE(heap, (heap).buf[_left_child_idx],                  \
                           (heap).buf[_priority_idx])) {                       \
        _priority_idx = _left_child_idx;                                       \
      }                                                                        \
      if (_right_child_idx < _n &&                                             \
          _HEAP_DO_COMPARE(heap, (heap).buf[_right_child_idx],                 \
                           (heap).buf[_priority_idx])) {                       \
        _priority_idx = _right_child_idx;                                      \
      }                                                                        \
      if (_priority_idx != _parent_idx) {                                      \
        typeof(*(heap).buf) _tmp = (heap).buf[_parent_idx];                    \
        (heap).buf[_parent_idx] = (heap).buf[_priority_idx];                   \
        (heap).buf[_priority_idx] = _tmp;                                      \
        _parent_idx = _priority_idx;                                           \
      } else {                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

/*
** HEAP_PUSH(heap, element);
*/
#define HEAP_PUSH(heap, element)                                               \
  do {                                                                         \
    if ((heap).count >= (heap).capacity) {                                     \
      (heap).capacity = (heap).capacity * 2 + 1;                               \
      (heap).buf = (typeof((heap).buf))realloc(                                \
          (heap).buf, (heap).capacity * sizeof(*(heap).buf));                  \
    }                                                                          \
    (heap).buf[(heap).count] = (element);                                      \
    _HEAP_SIFT_UP(heap, (heap).count);                                         \
    (heap).count++;                                                            \
  } while (0)

#define _GET_HEAP_PEEK(_1, _2, NAME, ...) NAME
#define _HEAP_PEEK_DEFAULT(heap, default)                                      \
  ((heap).count > 0 ? *(heap).buf : (default))
#define _HEAP_PEEK_ZERO(heap)                                                  \
  ((heap).count > 0 ? *(heap).buf : (typeof(*(heap).buf))0)

/*
** HEAP_PEEK(heap);
** HEAP_PEEK(heap, default);
*/
#define HEAP_PEEK(...)                                                         \
  _GET_HEAP_PEEK(__VA_ARGS__, _HEAP_PEEK_DEFAULT, _HEAP_PEEK_ZERO)(__VA_ARGS__)

#define _GET_HEAP_POP(_1, _2, NAME, ...) NAME
#define _HEAP_POP_DEFAULT(heap, default)                                       \
  ({                                                                           \
    typeof(*(heap).buf) _result;                                               \
    int _popped = 0;                                                           \
    if ((heap).count > 0) {                                                    \
      _result = *(heap).buf;                                                   \
      (heap).count--;                                                          \
      if ((heap).count > 0) {                                                  \
        *(heap).buf = (heap).buf[(heap).count];                                \
        _HEAP_SIFT_DOWN(heap);                                                 \
      }                                                                        \
      _popped = 1;                                                             \
    }                                                                          \
    if (!_popped) {                                                            \
      _result = (default);                                                     \
    }                                                                          \
    _result;                                                                   \
  })
#define _HEAP_POP_ZERO(heap) _HEAP_POP_DEFAULT(heap, (typeof(*(heap).buf))0)

/*
** HEAP_POP(heap);
** HEAP_POP(heap, default);
*/
#define HEAP_POP(...)                                                          \
  _GET_HEAP_POP(__VA_ARGS__, _HEAP_POP_DEFAULT, _HEAP_POP_ZERO)(__VA_ARGS__)

/*
** HEAP_FREE(heap);
** SAFETY: double free if used after heap creation without element insertion
*/
#define HEAP_FREE(heap)                                                        \
  do {                                                                         \
    free((heap).buf);                                                          \
    (heap).buf = NULL;                                                         \
    (heap).count = 0;                                                          \
    (heap).capacity = 0;                                                       \
    (heap).cmp = NULL;                                                         \
  } while (0)

/*
** HEAP_DBG(heap, format);
*/
#define HEAP_DBG(heap, fmt)                                                    \
  do {                                                                         \
    printf("Heap (count=%zu, capacity=%zu, cmp=%p): [", (heap).count,          \
           (heap).capacity, (void *)(heap).cmp);                               \
    for (size_t i = 0; i < (heap).count; i++) {                                \
      printf(fmt, (heap).buf[i]);                                              \
      if (i < (heap).count - 1) {                                              \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // HEAP_H_
