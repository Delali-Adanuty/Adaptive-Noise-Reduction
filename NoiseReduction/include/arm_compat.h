#ifndef ARM_COMPAT_H
#define ARM_COMPAT_H

#include <stdint.h>

#ifndef __CLZ
#define __CLZ(x) __builtin_clz(x)
#endif

#ifndef __SSAT
static inline int32_t __SSAT(int32_t val, int32_t sat)
{
    int32_t max = (1 << (sat - 1)) - 1;
    int32_t min = -(1 << (sat - 1));

    if (val > max) return max;
    if (val < min) return min;
    return val;
}
#endif

#endif