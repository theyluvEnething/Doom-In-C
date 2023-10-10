#ifndef VECTOR2_H
#define VECTOR2_H

#define vector2(x, y) (struct vector2){x, y}
#define vec2(x, y) (struct vector2){(double)x, (double)y}

struct vector2 {
    double x;
    double y;
};

struct vector2 create_vector2(double x, double y);
struct vector2* create_vector2_pointer(struct vector2* vec);
struct vector2 add_vector2(struct vector2 a, struct vector2 b);
struct vector2 multiply_vector2(struct vector2 a, double multiplier);
struct vector2 normalize_vector(struct vector2 a);
double magnitude(struct vector2 vec);


#endif