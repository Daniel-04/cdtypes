#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>

/*
** default min heap
*/
#define _heap_default_compare(a, b) ((a) < (b))

/*
** heap_t(type) heap = {0}
** heap_t(type) heap = {.cmp = c_function}
*/
#define heap_t(type)                                                           \
  struct {                                                                     \
    type *buf;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
    int (*cmp)(type a, type b);                                                \
  }

#define _heap_do_compare(heap, a, b)                                           \
  ((heap)->cmp ? (heap)->cmp(a, b) : _heap_default_compare(a, b))

#define _heap_sift_up(heap, start_index)                                       \
  do {                                                                         \
    size_t _child_idx = (start_index);                                         \
    while (_child_idx > 0) {                                                   \
      size_t _parent_idx = (_child_idx - 1) / 2;                               \
      if (_heap_do_compare(heap, (heap)->buf[_child_idx],                      \
                           (heap)->buf[_parent_idx])) {                        \
        typeof(*(heap)->buf) _tmp = (heap)->buf[_child_idx];                   \
        (heap)->buf[_child_idx] = (heap)->buf[_parent_idx];                    \
        (heap)->buf[_parent_idx] = _tmp;                                       \
        _child_idx = _parent_idx;                                              \
      } else {                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define _heap_sift_down(heap)                                                  \
  do {                                                                         \
    if ((heap)->count <= 1)                                                    \
      break;                                                                   \
    size_t _parent_idx = 0;                                                    \
    size_t _n = (heap)->count;                                                 \
    while (1) {                                                                \
      size_t _left_child_idx = 2 * _parent_idx + 1;                            \
      size_t _right_child_idx = 2 * _parent_idx + 2;                           \
      size_t _priority_idx = _parent_idx;                                      \
      if (_left_child_idx < _n &&                                              \
          _heap_do_compare(heap, (heap)->buf[_left_child_idx],                 \
                           (heap)->buf[_priority_idx])) {                      \
        _priority_idx = _left_child_idx;                                       \
      }                                                                        \
      if (_right_child_idx < _n &&                                             \
          _heap_do_compare(heap, (heap)->buf[_right_child_idx],                \
                           (heap)->buf[_priority_idx])) {                      \
        _priority_idx = _right_child_idx;                                      \
      }                                                                        \
      if (_priority_idx != _parent_idx) {                                      \
        typeof(*(heap)->buf) _tmp = (heap)->buf[_parent_idx];                  \
        (heap)->buf[_parent_idx] = (heap)->buf[_priority_idx];                 \
        (heap)->buf[_priority_idx] = _tmp;                                     \
        _parent_idx = _priority_idx;                                           \
      } else {                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

/*
** heap_push(&heap, element);
*/
#define heap_push(heap, element)                                               \
  do {                                                                         \
    if ((heap)->count >= (heap)->capacity) {                                   \
      (heap)->capacity = (heap)->capacity * 2 + 1;                             \
      (heap)->buf = (typeof((heap)->buf))realloc(                              \
          (heap)->buf, (heap)->capacity * sizeof(*(heap)->buf));               \
    }                                                                          \
    (heap)->buf[(heap)->count] = (element);                                    \
    _heap_sift_up(heap, (heap)->count);                                        \
    (heap)->count++;                                                           \
  } while (0)

#define _get_heap_peek(_1, _2, NAME, ...) NAME
#define _heap_peek_default(heap, default)                                      \
  ((heap)->count > 0 ? *(heap)->buf : (default))
#define _heap_peek_zero(heap)                                                  \
  ((heap)->count > 0 ? *(heap)->buf : (typeof(*(heap)->buf))0)

/*
** heap_peek(&heap);
** heap_peek(&heap, default);
*/
#define heap_peek(...)                                                         \
  _get_heap_peek(__VA_ARGS__, _heap_peek_default, _heap_peek_zero)(__VA_ARGS__)

#define _get_heap_pop(_1, _2, NAME, ...) NAME
#define _heap_pop_default(heap, default)                                       \
  ({                                                                           \
    typeof(*(heap)->buf) _result;                                              \
    int _popped = 0;                                                           \
    if ((heap)->count > 0) {                                                   \
      _result = *(heap)->buf;                                                  \
      (heap)->count--;                                                         \
      if ((heap)->count > 0) {                                                 \
        *(heap)->buf = (heap)->buf[(heap)->count];                             \
        _heap_sift_down(heap);                                                 \
      }                                                                        \
      _popped = 1;                                                             \
    }                                                                          \
    if (!_popped) {                                                            \
      _result = (default);                                                     \
    }                                                                          \
    _result;                                                                   \
  })
#define _heap_pop_zero(heap) _heap_pop_default(heap, (typeof(*(heap)->buf))0)

/*
** heap_pop(&heap);
** heap_pop(&heap, default);
*/
#define heap_pop(...)                                                          \
  _get_heap_pop(__VA_ARGS__, _heap_pop_default, _heap_pop_zero)(__VA_ARGS__)

/*
** heap_free(&heap);
** SAFETY: double free if used after heap creation without element insertion
*/
#define heap_free(heap)                                                        \
  do {                                                                         \
    free((heap)->buf);                                                         \
    (heap)->buf = NULL;                                                        \
    (heap)->count = 0;                                                         \
    (heap)->capacity = 0;                                                      \
    (heap)->cmp = NULL;                                                        \
  } while (0)

/*
** heap_dbg(&heap, format);
*/
#define heap_dbg(heap, fmt)                                                    \
  do {                                                                         \
    printf("Heap (count=%zu, capacity=%zu, cmp=%p): [", (heap)->count,         \
           (heap)->capacity, (void *)(heap)->cmp);                             \
    for (size_t i = 0; i < (heap)->count; i++) {                               \
      printf(fmt, (heap)->buf[i]);                                             \
      if (i < (heap)->count - 1) {                                             \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // HEAP_H_
