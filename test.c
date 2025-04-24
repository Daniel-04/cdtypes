#include "heap.h"
#include "map.h"
#include "queue.h"
#include "vec.h"

int cmp(int a, int b) { return a > b; }
size_t nohash(float f) { return (size_t)f; }

int main() {
  // vector test
  VEC_T(int) nums = {0};
  for (int i = 0; i < 10; i++) {
    VEC_PUSH(nums, i);
  }

  VEC_DBG(nums, "%d");

  int i = VEC_POP(nums);

  VEC_DBG(nums, "%d");

  while ((i = VEC_POP(nums, -1)) != -1) {
    VEC_DBG(nums, "%d");
  }

  VEC_FREE(nums);

  // heap test
  HEAP_T(int) heap = {0};

  HEAP_PUSH(heap, 5);
  HEAP_PUSH(heap, 1);
  HEAP_PUSH(heap, 8);
  HEAP_PUSH(heap, 3);
  HEAP_PUSH(heap, 9);
  HEAP_PUSH(heap, 2);

  HEAP_DBG(heap, "%d");

  int h = HEAP_POP(heap);

  HEAP_DBG(heap, "%d");

  while ((h = HEAP_POP(heap, -1)) != -1) {
    HEAP_DBG(heap, "%d");
  }

  HEAP_FREE(heap);

  // sort (descending)
  const int max = 50;
  srand(42);

  HEAP_T(int) m_heap = {.cmp = cmp};
  for (int i = 0; i < max; i++) {
    HEAP_PUSH(m_heap, rand() % max);
  }
  HEAP_DBG(m_heap, "%d");

  int curr;
  VEC_T(int) s_vec = {0};
  while ((curr = HEAP_POP(m_heap, -1)) != -1) {
    VEC_PUSH(s_vec, curr);
  }
  VEC_DBG(s_vec, "%d");

  HEAP_FREE(m_heap);
  VEC_FREE(s_vec);

  HEAP_DBG(m_heap, "%d");
  VEC_DBG(s_vec, "%d");

  // map test
  const float c = 42.93;
  MAP_T(float, int) hashmap = {.hash = nohash};
  MAP_DBG(hashmap, "%f", "%d");
  MAP_REMOVE(hashmap, c);

  for (int i = 0; i < 10; i++) {
    float f = i * c;
    MAP_INSERT(hashmap, f, i);
    MAP_DBG(hashmap, "%f", "%d");
  }
  MAP_DBG(hashmap, "%f", "%d");

  printf("%f = %d\n", 5 * c, MAP_GET(hashmap, 5 * c));
  printf("%f = %d\n", 10 * c, MAP_GET(hashmap, 10 * c));
  printf("%f = %d\n", 10 * c, MAP_GET(hashmap, 10 * c, -1));
  MAP_REMOVE(hashmap, 10 * c);

  for (int i = 0; i < 10; i++) {
    MAP_REMOVE(hashmap, i * c);
    MAP_DBG(hashmap, "%f", "%d");
  }

  for (int i = 0; i < 500; i++) {
    MAP_INSERT(hashmap, i * c, i);
  }

  MAP_FREE(hashmap);
  MAP_DBG(hashmap, "%f", "%d");

  // queue test
  QUEUE_T(int) queue = {0};
  for (int i = -7; i < 0; i++) {
    QUEUE_PUSH_BACK(queue, i);
  }

  QUEUE_DBG(queue, "%d");

  for (int i = 0; i < 7; i++) {
    QUEUE_POP_FRONT(queue);
    QUEUE_PUSH_BACK(queue, i);
    printf("<- %d, %d <-: ", QUEUE_PEEK_FRONT(queue), QUEUE_PEEK_BACK(queue));
    QUEUE_DBG(queue, "%d");
  }

  for (int i = 0; i < 10; i++) {
    QUEUE_PUSH_FRONT(queue, i);
    QUEUE_DBG(queue, "%d");
  }
  QUEUE_DBG(queue, "%d");

  while ((i = QUEUE_POP_BACK(queue, -1)) != -1) {
    QUEUE_DBG(queue, "%d");
  }

  QUEUE_FREE(queue);
  QUEUE_DBG(queue, "%d");

  return 0;
}
