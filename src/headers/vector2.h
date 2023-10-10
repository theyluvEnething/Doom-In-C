#ifndef VECTOR2_H
#define VECTOR2_H

#define vector2(x, y) (v2){x, y}
#define vec2(x, y) (v2){(int)x, (int)y}

typedef struct {
    int x, y;
} v2;

typedef v2 vector2;

v2 create_v2(double x, double y);
v2* create_v2_pointer(v2* vec);
v2 add_v2(v2 a, v2 b);
v2 multiply_v2(v2 a, double multiplier);
v2 normalize_vector(v2 a);
double magnitude(v2 vec);


#endif