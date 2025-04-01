#include "vec.h"

int main() {
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
  return 0;
}
