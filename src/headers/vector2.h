#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct vector2 { f32 x, y;} v2;
typedef struct vector2i { int x, y;} v2i;

#define vec2(x, y) (v2){x, y}
#define vec2i(x, y) (v2i){(int)x, (int)y}
#define v2(x, y) (v2){x, y}
#define v2i(x, y) (v2i){(int)x, (int)y}
#define lineseg(vec0, vec1) (lineseg){vec0, vec1}

#endif