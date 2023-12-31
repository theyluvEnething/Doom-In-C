#ifndef DEFS_H
#define DEFS_H

#include "stdint.h"
#include <stddef.h>

#include <math.h>
#ifdef NAN
/* NAN is supported */
#endif
#define ifnan(_x, _alt) ({ __typeof__(_x) __x = (_x); isnan(__x) ? (_alt) : __x; })


#ifdef INFINITY
/* INFINITY is supported */
#endif

#ifndef min
#define min(a, b) ((a > b) ? b : a)
#endif
#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#define clamp(x, minVal, maxVal) (max(min(x, maxVal), minVal)) 
#define distance(x0, y0, x1, y1) (sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)))

typedef float    f32;
typedef double   f64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef size_t   usize;

#endif