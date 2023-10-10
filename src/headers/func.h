#ifndef FUNC_H
#define FUNC_H

#include <stdarg.h>

#define PI (3.14159265358979323846)

void print_endl();
void print_warning(const char* message, ...);
void print_error(const char* message, ...);
void print_info(const char* message, ...);

int sign(double x);

float distancef(int x1, int y1, int x2, int y2);
float clampf(float d, float min, float max);
float absf(float x);

int distanceI(int x1, int y1, int x2, int y2);
int clampI(float d, float min, float max);
int absI(float x);
int minI(float a, float b);
int maxI(float a, float b);

long double esin(double angle);
long double ecos(double angle);

#endif