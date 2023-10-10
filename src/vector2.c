#include <stdio.h>
#include <math.h>

#include "headers/vector2.h"

vector2 create_vector2(double x, double y) {
    return (vector2){x, y};
}
vector2* create_vector2_pointer(vector2* vec) {
    return vec;
}
vector2 add_vector2(vector2 a, vector2 b) {
    return (vector2){a.x+b.x, a.y+b.y};
}
vector2 multiply_vector2(vector2 a, double multiplier) {
    vector2 temp; 
    temp.x = (double)(a.x*multiplier);
    temp.y = (double)(a.y*multiplier);
    return temp;
}

vector2 normalize_vector(vector2 vec) {
    double length = sqrt((vec.x*vec.x) + (vec.y*vec.y));
    return multiply_vector2(vec, 1/length);
}

double magnitude(vector2 vec) {
    return sqrt((vec.x*vec.x)+(vec.y*vec.y));
}
