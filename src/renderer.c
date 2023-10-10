#include <stdbool.h>
#include <Windows.h>
#include <math.h>

#include "headers/func.h"
#include "headers/frame.h"
#include "headers/vector2.h"
#include "headers/renderer.h"
#include "headers/player.h"

int get_direction(v2 point1, v2 point2);


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

void draw_pixel(v2 pos, int color, struct frame* frame) {
    frame->pixels[(int)(pos.x+pos.y*frame->width)%(frame->width*frame->height)] = color;
}

bool is_minimized(struct frame* frame) {
    return frame->width == 0 || frame->height == 0;
}

bool check_overlap(v2 pos, struct frame* frame) {
    return (pos.x < 0 || pos.x >= frame->width || pos.y < 0 || pos.y >= frame->height);
}

void draw_vert_line(v2 pos, int height, int color, struct frame* frame) {
    for (int y = pos.y; y < pos.y+height; y++) {
        frame->pixels[y*frame->width + pos.x] = color;
    }
}

void render() {

}

v2 rotate(v2 v, f32 a) {
    return (v2) {
        (v.x * cos(a)) - (v.y * sin(a)),
        (v.x * sin(a)) + (v.y * cos(a)),
    };
}

v2 world_pos_to_camera(v2 p, struct player player) {
    const v2 u = { p.x - player.pos.x, p.y - player.pos.y};
    return (v2) {
        u.x * sinf(player.lookangle) - u.y * cosf(player.lookangle),
        u.x * cosf(player.lookangle) - u.x * sinf(player.lookangle)
    };
}

int screen_angle_to_x(f32 a, struct frame frame) {
    const f32 a_pi4 = (((a + (HFOV / 2.0f)) / HFOV) * PI_2) + PI_4;
    return (int) (((int) (frame.width / 2)) * ((1.0f - tan(a_pi4))));
}
