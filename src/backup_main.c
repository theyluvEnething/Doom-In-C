#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define MAIN_SCREEN_WIDTH 1080
#define MAIN_SCREEN_HEIGHT 720
#define DEBUG_SCREEN_WIDTH 800
#define DEBUG_SCREEN_HEIGHT 500

#define MAX_POLYS   10
#define MAX_VERTS   8

struct frame {
    int width;
    int height;
    uint32_t* pixels;
};

typedef float    f32;
typedef double   f64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef size_t   usize;

typedef struct vector2 { f32 x, y;} v2;
typedef struct vector2i { int x, y;} v2i;

#define vec2(x, y) (v2){x, y}
#define vec2i(x, y) (v2i){(int)x, (int)y}
#define v2(x, y) (v2){x, y}
#define v2i(x, y) (v2i){(int)x, (int)y}
#define lineseg(vec0, vec1) (lineseg){vec0, vec1}

typedef struct {
    v2 p0, p1;
} lineseg;

typedef struct {
    v2 vert[MAX_VERTS];
    u8 vertCount;
    f32 height;
    f32 curDist;
} wall;

typedef struct {
    v2 vert[4];
    f32 distanceFromCamera;
    u8 index;
} Sector;

typedef struct {
    f32 lookangle;
    f32 stepWave;
    v2 pos, pPos;
} Player;

wall walls[MAX_POLYS];
Player player = {0};



static void print_info(const char* message, ...);
static void print_warning(const char* message, ...);
static void print_error(const char* message, ...);

/* FRAME */
struct frame frame = {0};
struct frame debug = {0};
/*========*/

/* INPUT */
bool keyboard[256] = {0};
struct {
    int x, y;
    uint_least8_t buttons;
} mouse;
enum 
{ 
    MOUSE_LEFT = 0b1, 
    MOUSE_MIDDLE = 0b10, 
    MOUSE_RIGHT = 0b100, 
    MOUSE_X1 = 0b1000, 
    MOUSE_X2 = 0b10000 
};
/*========*/

/* WINAPI32 */
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;

static BITMAPINFO debug_bitmap_info;
static HBITMAP debug_bitmap = 0;
static HDC debug_device_context = 0;

/* WINDOW */
static bool mn_wnd_running = true; static bool mn_wnd_minimized = false;
static bool db_wnd_running = true; static bool db_wnd_minimized = false;

#define FPS 60
const int frame_duration = 1000/FPS;
clock_t pTime;

static void HandleKeyboardInput(bool keyboard[]);

void Initialize() {
    player.lookangle = 0.42;
    player.pos.x = 451.96;
    player.pos.y = 209.24;

    walls[0].vert[0].x = 141.00;
    walls[0].vert[0].y = 84.00;
    walls[0].vert[1].x = 496.00;
    walls[0].vert[1].y = 81.00;
    walls[0].vert[2].x = 553.00;
    walls[0].vert[2].y = 136.00;
    walls[0].vert[3].x = 135.00;
    walls[0].vert[3].y = 132.00;
    walls[0].vert[4].x = 141.00;
    walls[0].vert[4].y = 84.00;
    walls[0].height = 50000;
    walls[0].vertCount = 5;
    walls[1].vert[0].x = 133.00;
    walls[1].vert[0].y = 441.00;
    walls[1].vert[1].x = 576.00;
    walls[1].vert[1].y = 438.00;
    walls[1].vert[2].x = 519.00;
    walls[1].vert[2].y = 493.00;
    walls[1].vert[3].x = 123.00;
    walls[1].vert[3].y = 497.00;
    walls[1].vert[4].x = 133.00;
    walls[1].vert[4].y = 441.00;
    walls[1].height = 50000;
    walls[1].vertCount = 5;
    walls[2].vert[0].x = 691.00;
    walls[2].vert[0].y = 165.00;
    walls[2].vert[1].x = 736.00;
    walls[2].vert[1].y = 183.00;
    walls[2].vert[2].x = 737.00;
    walls[2].vert[2].y = 229.00;
    walls[2].vert[3].x = 697.00;
    walls[2].vert[3].y = 247.00;
    walls[2].vert[4].x = 656.00;
    walls[2].vert[4].y = 222.00;
    walls[2].vert[5].x = 653.00;
    walls[2].vert[5].y = 183.00;
    walls[2].vert[6].x = 691.00;
    walls[2].vert[6].y = 165.00;
    walls[2].height = 10000;
    walls[2].vertCount = 7;
    walls[3].vert[0].x = 698.00;
    walls[3].vert[0].y = 330.00;
    walls[3].vert[1].x = 741.00;
    walls[3].vert[1].y = 350.00;
    walls[3].vert[2].x = 740.00;
    walls[3].vert[2].y = 392.00;
    walls[3].vert[3].x = 699.00;
    walls[3].vert[3].y = 414.00;
    walls[3].vert[4].x = 654.00;
    walls[3].vert[4].y = 384.00;
    walls[3].vert[5].x = 652.00;
    walls[3].vert[5].y = 348.00;
    walls[3].vert[6].x = 698.00;
    walls[3].vert[6].y = 330.00;
    walls[3].height = 10000;
    walls[3].vertCount = 7;
    walls[4].vert[0].x = 419.00;
    walls[4].vert[0].y = 311.00;
    walls[4].vert[1].x = 461.00;
    walls[4].vert[1].y = 311.00;
    walls[4].vert[2].x = 404.00;
    walls[4].vert[2].y = 397.00;
    walls[4].vert[3].x = 346.00;
    walls[4].vert[3].y = 395.00;
    walls[4].vert[4].x = 348.00;
    walls[4].vert[4].y = 337.00;
    walls[4].vert[5].x = 419.00;
    walls[4].vert[5].y = 311.00;
    walls[4].height = 50000;
    walls[4].vertCount = 6;
    walls[5].vert[0].x = 897.00;
    walls[5].vert[0].y = 98.00;
    walls[5].vert[1].x = 1079.00;
    walls[5].vert[1].y = 294.00;
    walls[5].vert[2].x = 1028.00;
    walls[5].vert[2].y = 297.00;
    walls[5].vert[3].x = 851.00;
    walls[5].vert[3].y = 96.00;
    walls[5].vert[4].x = 897.00;
    walls[5].vert[4].y = 98.00;
    walls[5].height = 10000;
    walls[5].vertCount = 5;
    walls[6].vert[0].x = 1025.00;
    walls[6].vert[0].y = 294.00;
    walls[6].vert[1].x = 1080.00;
    walls[6].vert[1].y = 292.00;
    walls[6].vert[2].x = 1149.00;
    walls[6].vert[2].y = 485.00;
    walls[6].vert[3].x = 1072.00;
    walls[6].vert[3].y = 485.00;
    walls[6].vert[4].x = 1025.00;
    walls[6].vert[4].y = 294.00;
    walls[6].height = 1000;
    walls[6].vertCount = 5;
    walls[7].vert[0].x = 1070.00;
    walls[7].vert[0].y = 483.00;
    walls[7].vert[1].x = 1148.00;
    walls[7].vert[1].y = 484.00;
    walls[7].vert[2].x = 913.00;
    walls[7].vert[2].y = 717.00;
    walls[7].vert[3].x = 847.00;
    walls[7].vert[3].y = 718.00;
    walls[7].vert[4].x = 1070.00;
    walls[7].vert[4].y = 483.00;
    walls[7].height = 1000;
    walls[7].vertCount = 5;
    walls[8].vert[0].x = 690.00;
    walls[8].vert[0].y = 658.00;
    walls[8].vert[1].x = 807.00;
    walls[8].vert[1].y = 789.00;
    walls[8].vert[2].x = 564.00;
    walls[8].vert[2].y = 789.00;
    walls[8].vert[3].x = 690.00;
    walls[8].vert[3].y = 658.00;
    walls[8].height = 10000;
    walls[8].vertCount = 4;
    walls[9].vert[0].x = 1306.00;
    walls[9].vert[0].y = 598.00;
    walls[9].vert[1].x = 1366.00;
    walls[9].vert[1].y = 624.00;
    walls[9].vert[2].x = 1369.00;
    walls[9].vert[2].y = 678.00;
    walls[9].vert[3].x = 1306.00;
    walls[9].vert[3].y = 713.00;
    walls[9].vert[4].x = 1245.00;
    walls[9].vert[4].y = 673.00;
    walls[9].vert[5].x = 1242.00;
    walls[9].vert[5].y = 623.00;
    walls[9].vert[6].x = 1306.00;
    walls[9].vert[6].y = 598.00;
    walls[9].height = 50000;
    walls[9].vertCount = 7;
}

static void verline(v2 pos, int height, int color, struct frame* frame) {
    for (int i = (u16)pos.y; i < (u16)pos.y+height; i++) frame->pixels[i*frame->width+(u16)pos.x] = color;
}

static void drawline(v2 v0, v2 v1, int width, int color, struct frame* frame) {

    int dx = abs(v1.x - v0.x);
    int dy = abs(v1.y - v0.y);
    bool slope = dy > dx;

    if (slope) {
        int temp = v0.x;
        v0.x = v0.y;
        v0.y = temp;

        temp = v1.x;
        v1.x = v1.y;
        v1.y = temp;
    }
    if (v0.x > v1.x) {
        int temp = v0.x;
        v0.x = v1.x;
        v1.x = temp;

        temp = v0.y;
        v0.y = v1.y;
        v1.y = temp;
    }

    dx = abs((int)(v1.x - v0.x));
    dy = abs((int)(v1.y - v0.y));
    float error = dx / 2;
    int y = v0.y;
    int ystep =  v0.y < v1.y ? 1 : -1;
    
    for (int x = v0.x; x < v1.x; x++) {

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

static float crossproduct(v2 vec1, v2 vec2) {
    return vec1.x* vec2.y - vec1.y * vec2.x;
}

static v2 Intersection(lineseg seg1, lineseg seg2) {
    v2 p;
    p.x = crossproduct(seg1.p1, seg1.p2);
    p.y = crossproduct(seg2.p1, seg2.p2);
    float det = crossproduct(vec2(seg1.p1.x-seg1.p2.x, seg1.p1.y-seg1.p2.y), vec2(seg2.p1.x-seg2.p2.x, seg2.p1.y-seg2.p2.y));
    p.x = crossproduct(vec2(p.x, seg1.p1.x-seg1.p2.x), vec2(p.y, seg2.p1.x-seg2.p2.x)) / det;
    p.y = crossproduct(vec2(p.x, seg1.p1.y-seg1.p2.y), vec2(p.y, seg2.p1.y-seg2.p2.y)) / det;

    return p;
}

static void background(int color, struct frame* frame) {
    for (int i = 0; i < frame->height; i++) {
        for (int j = 0; j < frame->width; j++) {
            frame->pixels[i*frame->width+j] = color;
        }
    }
}

static bool is_minimized(struct frame* frame) { 
    return frame->width == 0 || frame->height == 0; 
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE pInstance, 
                   PSTR cmd,
                   int showCmd) 
{
    printf("\n=========================================\n");

    Initialize();


    float mn_ratio = MAIN_SCREEN_WIDTH / (float)MAIN_SCREEN_HEIGHT;
    float db_ratio = DEBUG_SCREEN_WIDTH / (float)DEBUG_SCREEN_HEIGHT;
    if (mn_ratio != db_ratio) {
        print_warning("Incompatible ratio between main window and debug window.\n");
    }

    /* INITIALIZE  */
    WNDCLASS game_wnd_class = {0};
    const char CLASS_NAME[] = "GameWindow";
    game_wnd_class.lpfnWndProc = GameWindowProc;
    game_wnd_class.hInstance = hInstance;
    game_wnd_class.lpszClassName = CLASS_NAME;
    game_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);
    
    WNDCLASS debug_wnd_class = {0};
    const char CLASS_NAME_d[] = "DebugWindow";
    debug_wnd_class.lpfnWndProc = DebugWindowProc;
    debug_wnd_class.hInstance = hInstance;
    debug_wnd_class.lpszClassName = CLASS_NAME_d;
    debug_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);


    int reg0 = RegisterClass(&game_wnd_class);
    int reg1 = RegisterClass(&debug_wnd_class);

    printf("GameWndClass: %i, DebugWndClass: %i\n", reg0, reg1);

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1;
    frame_bitmap_info.bmiHeader.biBitCount = 32;
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);

    debug_bitmap_info.bmiHeader.biSize = sizeof(debug_bitmap_info.bmiHeader);
    debug_bitmap_info.bmiHeader.biPlanes = 1;
    debug_bitmap_info.bmiHeader.biBitCount = 32;
    debug_bitmap_info.bmiHeader.biCompression = BI_RGB;
    debug_device_context = CreateCompatibleDC(0);


    /* CREATE WINDOW  */
    HWND game_hwnd = CreateWindowEx(0, CLASS_NAME, "Doom",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                MAIN_SCREEN_WIDTH, MAIN_SCREEN_HEIGHT,
                                0, 0, hInstance, 0);

    printf("Initializing Game Window. (%i | %i)\n", frame.width, frame.height);

    HWND debug_hwnd = CreateWindowEx(0, CLASS_NAME_d, "Debug",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                DEBUG_SCREEN_WIDTH, DEBUG_SCREEN_HEIGHT,
                                0, 0, hInstance, 0);

    printf("Initializing Debug Window. (%i | %i)\n", debug.width, debug.height);
    
    if (game_hwnd == NULL || debug_hwnd == NULL) {
        printf("FAILED!\n");
        return 1;
    }


    /* GAME LOOP  */
    ShowWindow(game_hwnd, showCmd);
    ShowWindow(debug_hwnd, showCmd);

    /* level */
    pTime = clock();
    while (mn_wnd_running || db_wnd_running) 
    {
        clock_t cTime = clock();
        double elapsed = (double)(cTime - pTime) / CLOCKS_PER_SEC * 1000;
        if (elapsed < frame_duration) continue;
        pTime = cTime;
        printf("Frame rendered at %.2lf ms\n", 1000/elapsed);

		static MSG message = { 0 };
		while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }        
      
        HandleKeyboardInput(keyboard);


        if (!is_minimized(&frame)) {
            background(0x000000, &frame);
            verline(vec2(200, 500), 100, 0xFFFFFF, &frame);

            for (int n = 0; n < MAX_VERTS; n++) {
                for (int i = 0; i < walls[n].vertCount; i++) {

                    v2 lstart = walls[n].vert[i];
                    v2 lend = walls[n].vert[i+1];
                    f32 height = -walls[n].height;

                    v2 dist1 = (v2) {
                        lstart.x - player.pos.x,
                        lstart.y - player.pos.y
                    }, dist2 = (v2) {
                        lend.x - player.pos.x,
                        lend.y - player.pos.y
                    };

                    f32 z1 = dist1.x*cos(player.lookangle) + dist1.y*sin(player.lookangle),
                        z2 = dist2.x*cos(player.lookangle) + dist2.y*sin(player.lookangle);


                    if (z1 < 0 || z2 < 0) continue;

                    // v2 i1 = Intersection(
                    //     lineseg(vec2(dist1.x, z1), vec2(dist2.x, z2)),
                    //     lineseg(vec2(-0.0001, 0.0001), vec2(-20, 5))
                    // ),
                    //    i2 = Intersection(
                    //     lineseg(vec2(dist1.x, z1), vec2(dist2.x, z2)),
                    //     lineseg(vec2(0.0001, 0.0001), vec2(20, 5))
                    // );

                    // if (z1 <= 0) {
                    //     if (i1.y > 0) {
                    //         dist1.x = i1.x;
                    //         z1 = i1.y;
                    //     } else {
                    //         dist1.x = i2.x;
                    //         z1 = i2.y;
                    //     }
                    // }
                    // if (z2 <= 0) {
                    //     if (i1.y > 0) {
                    //         dist1.x = i1.x;
                    //         z2 = i1.y;
                    //     } else {
                    //         dist1.x = i2.x;
                    //         z2 = i2.y;
                    //     }
                    // }



                    dist1.x = dist1.x * sin(player.lookangle) - dist1.y * cos(player.lookangle);
                    dist2.x = dist2.x * sin(player.lookangle) - dist2.y * cos(player.lookangle);

                    const f32 widthRatio = MAIN_SCREEN_WIDTH / 2,
                              heightRatio = (MAIN_SCREEN_WIDTH * MAIN_SCREEN_HEIGHT) / 60.0,
                              centerScreenH = MAIN_SCREEN_HEIGHT / 2,
                              centerScreenW = MAIN_SCREEN_WIDTH / 2;


                    const f32 x1 = -dist1.x * widthRatio / z1,
                              x2 = -dist2.x * widthRatio / z2,
                              y1a = (height - heightRatio) / z1,
                              y1b = heightRatio / z1,
                              y2a = (height - heightRatio) / z2,
                              y2b = heightRatio / z2;

                    drawline(vec2(centerScreenW + x1, centerScreenH + y1a), vec2(centerScreenW + x2, centerScreenH + y2a), 1, 0xFF0000, &frame);
                    drawline(vec2(centerScreenW + x1, centerScreenH + y1b), vec2(centerScreenW + x2, centerScreenH + y2b), 1, 0xFF0000, &frame);
                    drawline(vec2(centerScreenW + x1, centerScreenH + y1a), vec2(centerScreenW + x1, centerScreenH + y1b), 1, 0xFF0000, &frame);
                    drawline(vec2(centerScreenW + x2, centerScreenH + y2a), vec2(centerScreenW + x2, centerScreenH + y2b), 1, 0xFF0000, &frame);

                }
            }




            InvalidateRect(game_hwnd, NULL, false);
            UpdateWindow(game_hwnd);
        }
        if (!is_minimized(&debug)) {
            
             
            //background(0x000000, &debug);
            
            InvalidateRect(debug_hwnd, NULL, false);
            UpdateWindow(debug_hwnd);
        }
    }
    printf("=========================================\n");
    PostQuitMessage(0);
    return 0;
}

static void HandleKeyboardInput(bool keyboard[]) {
    for (int i = 0; i < 256; i++) {
        if (keyboard[i]) printf("[%c | %i] was pressed\n", i, i);
    }

    if (keyboard['A']) {

    }
    if (keyboard['D']) {

    }
    if (keyboard['S']) {

    }
    if (keyboard['W']) {

    }

    return;
}

LRESULT CALLBACK GameWindowProc(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    static bool has_focus = true;
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frame_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint);
        } break;

        /* INIT PIXELS */
        case WM_SIZE: {
            frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (frame_bitmap) DeleteObject(frame_bitmap);
            frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, (void**)&frame.pixels, 0, 0);
            SelectObject(frame_device_context, frame_bitmap);

            frame.width = LOWORD(lParam);
            frame.height = HIWORD(lParam);
        } break;


         /* IDK (Close..?) */
        case WM_DESTROY:
        {
            mn_wnd_running = false;
            db_wnd_running = false;
            //PostQuitMessage(0);
            return 0;
        } break;     

        case WM_SETFOCUS: has_focus = true; break;

        case WM_KILLFOCUS: {
            printf("Debug Window is now in focus.\n");
			has_focus = false;
			memset(keyboard, 0, 256 * sizeof(keyboard[0]));
			mouse.buttons = 0;
		} break;

        /* KEYBOARD */
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
		case WM_KEYDOWN:
        case WM_KEYUP:
        {
			if (!has_focus)
                break;

            static bool key_is_down, key_was_down;
            key_is_down  = ((lParam & (1 << 31)) == 0);
            key_was_down = ((lParam & (1 << 30)) != 0);
            keyboard[(uint8_t)wParam] = key_is_down;


            switch(wParam) 
            {
                case VK_ESCAPE: 
                    mn_wnd_running = false; 
                    db_wnd_running = false;
                    PostQuitMessage(0);
                break;  
            }
        } break;

        /* MOUSE */
        case WM_MOUSEMOVE: {
			mouse.x = LOWORD(lParam);
			mouse.y = frame.height - 1 - HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: mouse.buttons |=  MOUSE_LEFT;   break;
		case WM_LBUTTONUP:   mouse.buttons &= ~MOUSE_LEFT;   break;
		case WM_MBUTTONDOWN: mouse.buttons |=  MOUSE_MIDDLE; break;
		case WM_MBUTTONUP:   mouse.buttons &= ~MOUSE_MIDDLE; break;
		case WM_RBUTTONDOWN: mouse.buttons |=  MOUSE_RIGHT;  break;
		case WM_RBUTTONUP:   mouse.buttons &= ~MOUSE_RIGHT;  break;

		case WM_XBUTTONDOWN: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons |= MOUSE_X1;
			} else { mouse.buttons |= MOUSE_X2; }
		} break;
		case WM_XBUTTONUP: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons &= ~MOUSE_X1;
			} else { mouse.buttons &= ~MOUSE_X2; }
		} break;

		case WM_MOUSEWHEEL: {
			printf("%s\n", wParam & 0b10000000000000000000000000000000 ? "Down" : "Up");
		} break;

         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}

LRESULT CALLBACK DebugWindowProc(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    static bool has_focus = true;
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   debug_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint);
        } break;

        /* INIT PIXELS */
        case WM_SIZE: {
            debug_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            debug_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (debug_bitmap) DeleteObject(debug_bitmap);
            debug_bitmap = CreateDIBSection(NULL, &debug_bitmap_info, DIB_RGB_COLORS, (void**)&debug.pixels, 0, 0);
            SelectObject(debug_device_context, debug_bitmap);

            debug.width = LOWORD(lParam);
            debug.height = HIWORD(lParam);
        } break;


         /* IDK (Close..?) */
        case WM_DESTROY:
        {
            db_wnd_running = false;
            //PostQuitMessage(0);
            return 0;
        } break;     

        case WM_SETFOCUS: has_focus = true; break;

        case WM_KILLFOCUS: {
            printf("Main Window is now in focus.\n");
			has_focus = false;
			memset(keyboard, 0, 256 * sizeof(keyboard[0]));
			mouse.buttons = 0;
		} break;

        

        /* KEYBOARD */
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
		case WM_KEYDOWN:
        case WM_KEYUP:
        {
			if (!has_focus)
                break;

            static bool key_is_down, key_was_down;
            key_is_down  = ((lParam & (1 << 31)) == 0);
            key_was_down = ((lParam & (1 << 30)) != 0);
            keyboard[(uint8_t)wParam] = key_is_down;

            switch(wParam) 
            {
                case VK_ESCAPE: 
                    mn_wnd_running = false; 
                    db_wnd_running = false;
                break;  
            }
        } break;


        /* MOUSE */
        /*
        case WM_MOUSEMOVE: {
			mouse.x = LOWORD(lParam);
			mouse.y = frame.height - 1 - HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: mouse.buttons |=  MOUSE_LEFT;   break;
		case WM_LBUTTONUP:   mouse.buttons &= ~MOUSE_LEFT;   break;
		case WM_MBUTTONDOWN: mouse.buttons |=  MOUSE_MIDDLE; break;
		case WM_MBUTTONUP:   mouse.buttons &= ~MOUSE_MIDDLE; break;
		case WM_RBUTTONDOWN: mouse.buttons |=  MOUSE_RIGHT;  break;
		case WM_RBUTTONUP:   mouse.buttons &= ~MOUSE_RIGHT;  break;

		case WM_XBUTTONDOWN: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons |= MOUSE_X1;
			} else { mouse.buttons |= MOUSE_X2; }
		} break;
		case WM_XBUTTONUP: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons &= ~MOUSE_X1;
			} else { mouse.buttons &= ~MOUSE_X2; }
		} break;

		case WM_MOUSEWHEEL: {
			printf("%s\n", wParam & 0b10000000000000000000000000000000 ? "Down" : "Up");
		} break;

        */

         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static void print_info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_MAGENTA "[INFO] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}
static void print_warning(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_YELLOW "[WARNING] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}
static void print_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf(ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET);
    vprintf(message, args);
    va_end(args);
}
