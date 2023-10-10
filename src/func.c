#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include "headers/func.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_endl() {
  printf("\n");
}

void print_info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_MAGENTA "[INFO] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}
void print_warning(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_YELLOW "[WARNING] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}
void print_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}



/* float */
float absf(float x) {
    if (x >= 0) return x;
    else return -x;
}
float distancef(int x1, int y1, int x2, int y2) {
    return (float)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
float clampf(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

/* INT */
int absI(float x) {
    if (x >= 0) return (int)x;
    else return (int)-x;
}

int distanceI(int x1, int y1, int x2, int y2) {
    return (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int clampI(float d, float min, float max) {
  const float t = (d < min) ? min : d;
  return (int)((t > max) ? max : t);
}

int minI(float a, float b) {
  return (a < b) ? a : b;
}

int maxI(float a, float b) {
  return (a > b) ? a : b;
}

int sign(double x) {
    return (x > 0) - (x < 0);
}



long double esin(double angle) {
  return 0;
    // mpfr_t r_angle, result;
    // mpfr_init2(r_angle, 10000);
    // mpfr_set_ld(r_angle, angle, MPFR_RNDN);

    // mpfr_init(result);
    // mpfr_sin(result, r_angle, MPFR_RNDN);

    // long double sinResult = mpfr_get_ld(result, MPFR_RNDN);

    // mpfr_clear(r_angle);
    // mpfr_clear(result);

    // return sinResult;
}

long double ecos(double angle) {
  return 1;
    // mpfr_t r_angle, result;
    // mpfr_init2(r_angle, PRECISION);
    // mpfr_set_ld(r_angle, angle, MPFR_RNDN);

    // mpfr_init(result);
    // mpfr_cos(result, r_angle, MPFR_RNDN);

    // long double cosResult = mpfr_get_ld(result, MPFR_RNDN);

    // mpfr_clear(r_angle);
    // mpfr_clear(result);

    // return cosResult;
}