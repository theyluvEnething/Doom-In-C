#include "headers/defs.h"
#include "headers/frame.h"
#include "headers/vector2.h"

#include <stdbool.h>

void verline(v2 pos, int height, int color, struct frame* frame) {
    if ((u16)pos.y+height > frame->height) return;
    for (int i = (u16)pos.y; i < (u16)pos.y+height; i++) frame->pixels[i*frame->width+(u16)pos.x] = color;
}

void drawline(v2 v0, v2 v1, int width, int color, struct frame* frame) {

    int x0 = v0.x, y0 = v0.y,
        x1 = v1.x, y1 = v1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    bool slope = dy > dx;

    if (slope) {
        int temp = x0;
        x0 = y0;
        y0 = temp;

        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    if (x0 > x1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    dx = abs((int)(x1 - x0));
    dy = abs((int)(y1 - y0));
    float error = dx / 2;
    int y = y0;
    int ystep =  y0 < y1 ? 1 : -1;
    
    for (int x = x0; x < x1; x++) {

        struct vector2 pos = slope ? vec2(y, x) : vec2(x, y);

        for (int i = -width/2; i < width/2+1; i++) {
            if (pos.x + i < 0 || pos.x + i > frame->width-1 || pos.y + i < 0 || pos.y + i > frame->height-1) continue;
            frame->pixels[(uint32_t)(pos.x+i+frame->width*pos.y)%(frame->height*frame->width)] = color;
            frame->pixels[(uint32_t)(pos.x+frame->width*(pos.y+i))] = color;
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }   
}

void background(int color, struct frame* frame) {
    for (int i = 0; i < frame->height; i++) {
        for (int j = 0; j < frame->width; j++) {
            frame->pixels[i*frame->width+j] = color;
        }
    }
}

bool is_minimized(struct frame* frame) { 
    return frame->width == 0 || frame->height == 0; 
}

v2 WorldPosToFramePos(v2 vec, f32 zoom, struct frame *frame) {
    f32 x = (vec.x/(10.0f/zoom))*frame->width,
        y = (vec.y/(10.0f/zoom))*frame->width;
    return vec2(x, y);
}
