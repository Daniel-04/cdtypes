#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
#include <stdlib.h>

#define FACTOR 0.7
#define FIRSTSIZE 64

/*
** MAP_T(key_type, val_type) hashmap = {.hash = h_function};
** NOTE: key type must be copy-able and support (==)
*/
#define MAP_T(key_type, val_type)                                              \
  struct {                                                                     \
    struct {                                                                   \
      key_type k;                                                              \
      val_type v;                                                              \
      int psl;                                                                 \
    } *buf;                                                                    \
    size_t count;                                                              \
    size_t capacity;                                                           \
    size_t (*hash)(key_type key);                                              \
  }

/*
** MAP_INSERT(map, key, value);
*/
#define MAP_INSERT(map, key, value)                                            \
  do {                                                                         \
    _MAP_RESIZE(map);                                                          \
    _MAP_INSERT(map, key, value);                                              \
  } while (0)

#define _MAP_INSERT(map, key, value)                                           \
  do {                                                                         \
    size_t _idx = (map).hash(key) % (map).capacity;                            \
    typeof(*(map).buf) _entry = {(key), (value), 0};                           \
    do {                                                                       \
      _entry.psl++;                                                            \
      if (_entry.psl > (map).buf[_idx].psl) {                                  \
        typeof(*(map).buf) tmp = (map).buf[_idx];                              \
        (map).buf[_idx] = _entry;                                              \
        _entry = tmp;                                                          \
      }                                                                        \
      _idx = (_idx + 1) % (map).capacity;                                      \
    } while (_entry.psl != 0);                                                 \
    (map).count++;                                                             \
  } while (0)

#define _MAP_RESIZE(map)                                                       \
  do {                                                                         \
    if ((map).count >= (map).capacity * FACTOR) {                              \
      size_t _old_capacity = (map).capacity;                                   \
      typeof(*(map).buf) *_old_buf = (map).buf;                                \
      (map).capacity = _old_capacity * 2 + FIRSTSIZE;                          \
      (map).buf = calloc((map).capacity, sizeof(*(map).buf));                  \
      (map).count = 0;                                                         \
      for (size_t i = 0; i < _old_capacity; ++i) {                             \
        if (_old_buf[i].psl != 0) {                                            \
          _MAP_INSERT(map, _old_buf[i].k, _old_buf[i].v);                      \
        }                                                                      \
      }                                                                        \
      free(_old_buf);                                                          \
    }                                                                          \
  } while (0)

#define _GET_MAP_GET(_1, _2, _3, NAME, ...) NAME
#define _MAP_GET_DEFAULT(map, key, default_val)                                \
  ({                                                                           \
    size_t _idx = (map).hash(key) % (map).capacity;                            \
    int _prev_psl = -1;                                                        \
    while ((key) != (map).buf[_idx].k && _prev_psl < (map).buf[_idx].psl) {    \
      _prev_psl = (map).buf[_idx].psl;                                         \
      _idx = (_idx + 1) % (map).capacity;                                      \
    }                                                                          \
    ((key) == (map).buf[_idx].k) ? (map).buf[_idx].v : (default_val);          \
  })
#define _MAP_GET_ZERO(map, key)                                                \
  ({                                                                           \
    size_t _idx = (map).hash(key) % (map).capacity;                            \
    int _prev_psl = -1;                                                        \
    while ((key) != (map).buf[_idx].k && _prev_psl < (map).buf[_idx].psl) {    \
      _prev_psl = (map).buf[_idx].psl;                                         \
      _idx = (_idx + 1) % (map).capacity;                                      \
    }                                                                          \
    ((key) == (map).buf[_idx].k) ? (map).buf[_idx].v                           \
                                 : (typeof((map).buf[_idx].v))0;               \
  })

/*
** MAP_GET(map, key);
** MAP_GET(map, key, default);
*/
#define MAP_GET(...)                                                           \
  _GET_MAP_GET(__VA_ARGS__, _MAP_GET_DEFAULT, _MAP_GET_ZERO)(__VA_ARGS__)

/*
** MAP_REMOVE(map, key);
*/
#define MAP_REMOVE(map, key)                                                   \
  do {                                                                         \
    size_t _idx = (map).hash(key) % (map).capacity;                            \
    int _prev_psl = -1;                                                        \
    while ((key) != (map).buf[_idx].k && _prev_psl < (map).buf[_idx].psl) {    \
      _prev_psl = (map).buf[_idx].psl;                                         \
      _idx = (_idx + 1) % (map).capacity;                                      \
    }                                                                          \
    if ((key) == (map).buf[_idx].k) {                                          \
      size_t _prev_idx = _idx;                                                 \
      _idx = (_idx + 1) % (map).capacity;                                      \
      while ((map).buf[_idx].psl > 1) {                                        \
        (map).buf[_prev_idx] = (map).buf[_idx];                                \
        (map).buf[_prev_idx].psl--;                                            \
        _prev_idx = _idx;                                                      \
        _idx = (_idx + 1) % (map).capacity;                                    \
      }                                                                        \
      (map).buf[_prev_idx].psl = 0;                                            \
      (map).count--;                                                           \
    }                                                                          \
  } while (0)

/*
** MAP_FREE(map);
** SAFETY: double free if used after map creation without element insertion
*/
#define MAP_FREE(map)                                                          \
  do {                                                                         \
    free((map).buf);                                                           \
    (map).buf = NULL;                                                          \
    (map).count = 0;                                                           \
    (map).capacity = 0;                                                        \
    (map).hash = NULL;                                                         \
  } while (0)

/*
** MAP_DBG(map, key_format, val_format);
*/
#define MAP_DBG(map, key_fmt, val_fmt)                                         \
  do {                                                                         \
    printf("Map (count=%zu, capacity=%zu, hash=%p): [", (map).count,           \
           (map).capacity, (void *)(map).hash);                                \
    for (size_t i = 0; i < (map).capacity; i++) {                              \
      if ((map).buf[i].psl != 0) {                                             \
        printf(key_fmt ":" val_fmt ", ", (map).buf[i].k, (map).buf[i].v);      \
      }                                                                        \
      if (i == (map).capacity - 1 && (map).count > 0) {                        \
        printf("\b\b");                                                        \
      }                                                                        \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif // MAP_H_
