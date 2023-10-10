#include <stdbool.h>
#include <Windows.h>
#include <math.h>

#include "headers/func.h"
#include "headers/frame.h"
#include "headers/vector2.h"
#include "headers/renderer.h"

int get_direction(struct vector2 point1, struct vector2 point2);


/* GENERAL METHODS */
void update_screen(HWND* window_handle) {
    InvalidateRect(*window_handle, NULL, false);
    UpdateWindow(*window_handle);
}

void background(int color, struct frame* frame) {
    for (int i = 0; i < frame->width*frame->height; i++) {
        frame->pixels[i] = color;
    }
}

void draw_pixel(struct vector2 pos, int color, struct frame* frame) {
    frame->pixels[(int)(pos.x+pos.y*frame->width)%(frame->width*frame->height)] = color;
}

bool is_minimized(struct frame* frame) {
    return frame->width == 0 || frame->height == 0;
}

bool check_overlap(struct vector2 pos, struct frame* frame) {
    return (pos.x < 0 || pos.x >= frame->width || pos.y < 0 || pos.y >= frame->height);
}



