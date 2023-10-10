#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <windows.h>
#include "frame.h"

void update_screen(HWND* window_handle);
void background(int color, struct frame* frame);
void draw_pixel(struct vector2 pos, int color, struct frame* frame);
bool is_minimized(struct frame* frame);
bool check_overlap(struct vector2 pos, struct frame* frame);

#endif