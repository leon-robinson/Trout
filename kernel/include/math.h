#ifndef __TROUT_MATH_H
#define __TROUT_MATH_H

#define ALIGN_DOWN(base, size)        ((base) & -((__typeof__ (base)) (size)))
#define ALIGN_UP(base, size)        ALIGN_DOWN ((base) + (size) - 1, (size))
#define PTR_ALIGN_UP(base, size) \
  ((__typeof__ (base)) ALIGN_UP ((uintptr_t) (base), (size)))

#endif