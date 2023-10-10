#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <stdarg.h>

#define PI (3.14159265358979323846)
#define PI_2 (PI/2)
#define PI_4 (PI/4)

#define sign(x) ((x >= 0) ? 1 : -1)
#define abs(x) ((x >= 0) ? x : -x)

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint64_t  u64;
typedef uint32_t  u32;
typedef float     f32;
typedef double    f64;

void print_endl();
void print_warning(const char* message, ...);
void print_error(const char* message, ...);
void print_info(const char* message, ...);

int clampI(float d, float min, float max);
float clampf(float d, float min, float max);

int distanceI(int x1, int y1, int x2, int y2);
float distancef(int x1, int y1, int x2, int y2);


#endif