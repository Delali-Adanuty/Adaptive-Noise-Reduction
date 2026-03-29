#ifndef CMSIS_COMPILER_H
#define CMSIS_COMPILER_H

#include <stdint.h>

// 1. Mock ARM Compiler Macros
#ifndef __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE static inline __attribute__((always_inline))
#endif
#ifndef __STATIC_INLINE
  #define __STATIC_INLINE static inline
#endif
#ifndef __ALIGNED
  #define __ALIGNED(x) __attribute__((aligned(x)))
#endif
#ifndef __PACKED
  #define __PACKED __attribute__((packed, aligned(1)))
#endif
#ifndef __WEAK
  #define __WEAK __attribute__((weak))
#endif
#ifndef __RESTRICT
  #define __RESTRICT __restrict
#endif

// 2. The "Missing" DSP Attribute
#ifndef ARM_DSP_ATTRIBUTE
  #define ARM_DSP_ATTRIBUTE 
#endif

#endif // CMSIS_COMPILER_H