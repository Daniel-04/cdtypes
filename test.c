#include "heap.h"
#include "map.h"
#include "vec.h"

int cmp(int a, int b) { return a > b; }
size_t nohash(float f) { return (size_t)f; }

int main() {
  // vector test
  vec_t(int) nums = {0};
  for (int i = 0; i < 10; i++) {
    vec_push(&nums, i);
  }

  vec_dbg(&nums, "%d");

  int i = vec_pop(&nums);

  vec_dbg(&nums, "%d");

  while ((i = vec_pop(&nums, -1) != -1)) {
    vec_dbg(&nums, "%d");
  }

  vec_free(&nums);

  // heap test
  heap_t(int) heap = {0};

  heap_push(&heap, 5);
  heap_push(&heap, 1);
  heap_push(&heap, 8);
  heap_push(&heap, 3);
  heap_push(&heap, 9);
  heap_push(&heap, 2);

  heap_dbg(&heap, "%d");

  int h = heap_pop(&heap);

  heap_dbg(&heap, "%d");

  while ((h = heap_pop(&heap, -1)) != -1) {
    heap_dbg(&heap, "%d");
  }

  heap_free(&heap);

  // sort (descending)
  const int max = 50;
  srand(42);

  heap_t(int) m_heap = {.cmp = cmp};
  for (int i = 0; i < max; i++) {
    heap_push(&m_heap, rand() % max);
  }
  heap_dbg(&m_heap, "%d");

  int curr;
  vec_t(int) s_vec = {0};
  while ((curr = heap_pop(&m_heap, -1)) != -1) {
    vec_push(&s_vec, curr);
  }
  vec_dbg(&s_vec, "%d");

  heap_free(&m_heap);
  vec_free(&s_vec);

  heap_dbg(&m_heap, "%d");
  vec_dbg(&s_vec, "%d");

  // map test
  map_t(float, int) hashmap = {.hash = nohash};
  map_dbg(&hashmap, "%f", "%d");

  const float c = 42.93;
  for (int i = 0; i < 10; i++) {
    float f = i * c;
    map_insert(&hashmap, f, i);
    map_dbg(&hashmap, "%f", "%d");
  }
  map_dbg(&hashmap, "%f", "%d");

  printf("%f = %d\n", 5 * c, map_get(&hashmap, 5 * c));
  printf("%f = %d\n", 10 * c, map_get(&hashmap, 10 * c));
  printf("%f = %d\n", 10 * c, map_get(&hashmap, 10 * c, -1));

  for (int i = 0; i < 10; i++) {
    map_remove(&hashmap, i * c);
    map_dbg(&hashmap, "%f", "%d");
  }

  for (int i = 0; i < 500; i++) {
    map_insert(&hashmap, i * c, i);
  }

  map_free(&hashmap);
  map_dbg(&hashmap, "%f", "%d");

  return 0;
}
