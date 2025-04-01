#include "heap.h"
#include "vec.h"

int main() {
  // vector test
  vec_t(int) nums = {0};
  for (int i = 0; i < 10; i++) {
    vec_append(&nums, i);
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

  return 0;
}
