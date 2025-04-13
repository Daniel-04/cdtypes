# Sufficiently generic and usable data types macros for C
### (abusing GNU extensions)

Each macro has a comment showing its usage, here is some code for vector:
``` c
VEC_T(int) nums = {0};
for(int i = 0; i < 10) {
    VEC_PUSH(nums, i);
}
int nine = VEC_POP(nums);
int five = nums.buf[5];
VEC_DBG(nums, "%d");
```

# Notes - (on no string support)
- All type arguments are assumed to be copied on assignment (=).
- The key type argument for the map macro is assumed to support equality (==).
