#include "headers/defs.h"
#include "headers/frame.h"
#include "headers/vector2.h"
#include "headers/level.h"

#include <stdio.h>
#include <stdbool.h>

void verline(v2 pos, int height, int color, struct frame* frame) {

    if (pos.x < 0 || pos.x > frame->width) return;
    u32 floor = ((pos.y > 0) ? pos.y : 0);
    if (floor + height < 0) return;
    u32 ceil = ((pos.y+height < frame->height) ? ceil = pos.y+height : frame->height);

    for (u32 i = floor; i < ceil; i++) {
        frame->pixels[(int)pos.x+frame->width*i] = color;
    }
}

void drawpixel(v2 pos, int color, struct frame* frame) {
    int px = pos.x+frame->width*pos.y;
    if (px < 0 || px > frame->width*frame->height) return;
    frame->pixels[px] = color;
}

void drawline(v2 v0, v2 v1, int width, int color, struct frame* frame) {

    // int x0 = clamp(v0.x, 0, frame->width),
    //     y0 = clamp(v0.y, 0, frame->height),
    //     x1 = clamp(v1.x, 0, frame->width),
    //     y1 = clamp(v1.y, 0, frame->height);

    int x0 = v0.x,
        y0 = v0.y,
        x1 = v1.x,
        y1 = v1.y;

    int dx;
    int dy;

    if (x1 > x0)
        dx = x1 - x0;
    else
        dx = x0 - x1;
    if (y1 > y0)
        dy = y1 - y0;
    else
        dy = y0 - y1;

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {

        if (width > 1) {
            for (int i = -width/2; i < width/2; i++) {
                if (0+width/2 < x0+i && x0+i < frame->width-width/2 && 0+width/2 < y0+i && y0+i < frame->height-width/2) {
                    u32 px = x0+frame->width*y0;
                    frame->pixels[px+i] = color;
                    frame->pixels[px+frame->width*i] = color;
                }
            }
        } else {
            if (0 < x0 && x0 < frame->width && 0 < y0 && y0 < frame->height) {
                u32 px = x0+frame->width*y0;
                frame->pixels[px] = color;
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
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

// fit lines to screen (probb. only needed for debug wnd)
v2 WorldPosToFramePos(v2 vec, f32 zoom, struct frame *frame) {
    f32 x = (vec.x/(10.0f/zoom))*frame->width,
        y = (vec.y/(10.0f/zoom))*frame->width;
    return vec2(x, y);
}
 
v2 AddCameraOffset(v2 vec, v2 camera) {
    return (v2){vec.x-camera.x, vec.y-camera.y};
}

// check for line intersections, returns (NAN, NAN) if there is none
v2 check_intersect(v2 a0, v2 a1, v2 b0, v2 b1) {
 const f32 d =
        ((a0.x - a1.x) * (b0.y - b1.y))
            - ((a0.y - a1.y) * (b0.x - b1.x));

    if (fabsf(d) < 0.000001f) { return (v2) { NAN, NAN }; }

    const f32
        t = (((a0.x - b0.x) * (b0.y - b1.y))
                - ((a0.y - b0.y) * (b0.x - b1.x))) / d,
        u = (((a0.x - b0.x) * (a0.y - a1.y))
                - ((a0.y - b0.y) * (a0.x - a1.x))) / d;
    return (t >= 0 && t <= 1 && u >= 0 && u <= 1) ?
        ((v2) {
            a0.x + (t * (a1.x - a0.x)),
            a0.y + (t * (a1.y - a0.y)) })
        : ((v2) { NAN, NAN });
}