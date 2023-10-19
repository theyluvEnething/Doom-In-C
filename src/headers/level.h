#ifndef LEVEL_H
#define LEVEL_H

#include "func.h"
#include "vector2.h"
#include "player.h"

// sector id for "no sector"
#define SECTOR_NONE 0
#define SECTOR_MAX 128

typedef struct wall {
    vector2 a, b;
    int portal;
} wall;

struct sector {
    u8 id;
    size_t firstwall, nwalls;
    f32 zfloor, zceil;
};

struct level {
    struct { struct sector arr[32]; size_t n; } sectors;
    struct { struct wall arr[128]; size_t n; } walls;
};

#endif