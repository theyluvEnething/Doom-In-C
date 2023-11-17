#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <windows.h>
#include "frame.h"
#include "defs.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define DEBUG_SCREEN_WIDTH 800
#define DEBUG_SCREEN_HEIGHT 500

void background(int color, struct frame* frame);
bool is_minimized(struct frame* frame);
void verline(v2 pos, int height, int color, struct frame* frame);
void drawline(v2 v0, v2 v1, int width, int color, struct frame* frame);
void drawpixel(v2 pos, int color, struct frame* frame);
void draw_center_circle(struct vector2 pos, int rad, int color, struct frame* frame);

v2 WorldPosToFramePos(v2 vec, f32 zoom, struct frame *frame);
v2 AddCameraOffset(v2 vec, v2 camera);

v2 check_intersect(v2 a0, v2 a1, v2 b0, v2 b1);
#endif