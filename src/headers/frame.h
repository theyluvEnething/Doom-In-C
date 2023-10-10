#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

struct frame {
    int width;
    int height;
    uint32_t* pixels;
};

#endif