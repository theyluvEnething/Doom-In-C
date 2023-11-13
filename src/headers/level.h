#ifndef LEVEL_H
#define LEVEL_H

#include "defs.h"
#include "vector2.h"
#include "renderer.h"


#define MAX_SECTORS 32
#define MAX_WALLS 256

typedef struct {
    v2 pA, pB;
} line;

typedef struct wall {
    v2 a, b;
    int portal;
    struct wall *next;
} wall;

struct sector {
    u8 id;
    f32 zfloor, zceil;
    struct { struct wall *fwall; usize n; } list;
};

struct level {
    struct { struct sector arr[128]; usize n; } sectors;
    u16 y_lo[SCREEN_WIDTH], y_hi[SCREEN_WIDTH];
};

int load_level(const char* path, struct level* level);

#endif