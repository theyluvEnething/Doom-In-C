#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <windows.h>
#include "frame.h"
#include "player.h"

#define MAIN_SCREEN_WIDTH 1080
#define MAIN_SCREEN_HEIGHT 720
#define DEBUG_SCREEN_WIDTH 800
#define DEBUG_SCREEN_HEIGHT 500

#define DEG2RAD(_d) ((_d) * (PI / 180.0f))
#define RAD2DEG(_d) ((_d) * (180.0f / PI))

#define EYE_Z 1.65f
#define HFOV DEG2RAD(90.0f)
#define VFOV 0.5f

#define ZNEAR 0.0001f
#define ZFAR  128.0f


void update_screen(HWND* window_handle);
void background(int color, struct frame* frame);
void draw_pixel(v2 pos, int color, struct frame* frame);
void draw_vert_line(v2 pos, int height, int color, struct frame* frame);
bool is_minimized(struct frame* frame);
bool check_overlap(v2 pos, struct frame* frame);

v2 rotate(v2 v, f32 a);
v2 world_pos_to_camera(v2 p, struct player player);
int screen_angle_to_x(f32 a, struct frame frame);
void render();


#endif