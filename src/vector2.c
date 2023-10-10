#include <stdio.h>
#include <math.h>

#include "headers/vector2.h"

struct vector2 create_vector2(double x, double y) {
    return (struct vector2){x, y};
}
struct vector2* create_vector2_pointer(struct vector2* vec) {
    return vec;
}

struct vector2 add_vector2(struct vector2 a, struct vector2 b) {
    return (struct vector2){a.x+b.x, a.y+b.y};
}

struct vector2 multiply_vector2(struct vector2 a, double multiplier) {
    struct vector2 temp; 
    temp.x = (double)(a.x*multiplier);
    temp.y = (double)(a.y*multiplier);
    // printf("a: (%i, %i) | ", a.x, a.y);
    // printf("multiplied: (%i, %i)\n", temp.x, temp.y);
    return temp;
    //return (struct vector2){a.x*multiplier, a.y*multiplier};
}

struct vector2 normalize_vector(struct vector2 vec) {
    double length = sqrt((vec.x*vec.x) + (vec.y*vec.y));
    return multiply_vector2(vec, 1/length);
}

double magnitude(struct vector2 vec) {
    return sqrt((vec.x*vec.x)+(vec.y*vec.y));
}
