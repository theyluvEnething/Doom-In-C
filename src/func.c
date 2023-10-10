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

float distancef(int x1, int y1, int x2, int y2) { return (float)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)); }
float clampf(float d, float min, float max) { const float t = d < min ? min : d; return t > max ? max : t; }
int distanceI(int x1, int y1, int x2, int y2) { return (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)); }
int clampI(float d, float min, float max) { const float t = (d < min) ? min : d; return (int)((t > max) ? max : t); }
