#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

#include "headers/lLib.h"
#include "headers/level.h"
#include "headers/vector2.h"
#include "headers/renderer.h"


#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define DEBUG_SCREEN_WIDTH 800
#define DEBUG_SCREEN_HEIGHT 500

#define MAX_POLYS   10
#define MAX_VERTS   8

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

struct level level = {0};

typedef struct {
    f32 lookangle;
    f32 speed;
    v2 pos, pPos;
} Player;

Player player = {0};

/* FRAME */
struct frame frame = {0};
struct frame debug = {0};
/*========*/

/* INPUT */
bool keyboard[256] = {0};
struct {
    int x, y;
    bool dragging;
    v2 pos, pPos;
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
f32 zoom = 1;

static void HandleKeyboardInput(bool keyboard[]);

void Initialize() {
    player.lookangle = 0;
    player.pos.x = 0;
    player.pos.y = 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE pInstance, 
                   PSTR cmd,
                   int showCmd) 
{
    printf("\n=========================================\n");

    Initialize();


    float mn_ratio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
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
                                SCREEN_WIDTH, SCREEN_HEIGHT,
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


    int ret = load_level("res/poly.txt", &level);
    if (ret != 0) { 
        printf("Error while loading level: %i", ret);
        return 0;
    }

    printf("\n");
    // DOESNT WORK SADLY
    for (int i = 0; i < level.sectors.arr[1].list.n; i++) {
        printf(" [%i, %i] ", (int)level.sectors.arr[1].list.fwall[i].a.x, (int)level.sectors.arr[1].list.fwall[i].a.y);
    }
    printf("\n");


    

    printf("Size Wall: {%u}!\n", sizeof(struct wall));
    
    for (int j = 0; j < level.sectors.n; j++) {
        printf("\n[%i] - Sector: %i Wall(s)\n > ", j, level.sectors.arr[j].list.n);
        wall *cwall = level.sectors.arr[j].list.fwall;
        while (cwall != NULL) {
            printf("[(%i|%i)-(%i|%i)] ",
                (int)cwall->a.x, (int)cwall->a.y,
                (int)cwall->b.x, (int)cwall->b.y);

            cwall = cwall->next;
        }
        printf("\n");
    }

    for (int j = 0; j < level.sectors.n; j++) {
        wall *cwall = level.sectors.arr[j].list.fwall;
        while (cwall != NULL) {
            
            wall *vwall = level.sectors.arr[j].list.fwall;
            while (vwall != NULL) { 
                v2 intersection = check_intersect(cwall->a, cwall->b, vwall->a, vwall->b);
                
                printf("Lines intersecting at: (%i, %i)\n", (int)intersection.x, (int)intersection.y);
                
                vwall = vwall->next;
            }
            cwall = cwall->next;
        }
    }

    printf("\nSucessfully started.\n");


    pTime = clock();

    bool wasDragging = false;
    v2 cameraOffset = (v2){0, 0};
    v2 dragDifference = (v2){0,0};

    while (mn_wnd_running || db_wnd_running) 
    {
        mouse.pos = (v2){mouse.x, mouse.y};
        clock_t cTime = clock();
        double elapsed = (double)(cTime - pTime) / CLOCKS_PER_SEC * 1000;
        if (elapsed < frame_duration) continue;
        pTime = cTime;
        //printf("Frame rendered at %.2lf ms\n", 1000/elapsed);

		static MSG message = { 0 };
		while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }        
      
        HandleKeyboardInput(keyboard);



        if (!is_minimized(&frame)) {
            background(0x000000, &frame);
            verline(vec2(200, 500), 100, 0xFFFFFF, &frame);


            drawline(vec2(DEBUG_SCREEN_WIDTH/2, DEBUG_SCREEN_HEIGHT/2), vec2(mouse.x, mouse.y), 1, 0xFFFFFF, &frame);
            verline(vec2(mouse.x, mouse.y), 100, 0xFFFFFF, &frame);
            verline(vec2(mouse.x-1, mouse.y), 100, 0xFFFFFF, &frame);
            verline(vec2(mouse.x+1, mouse.y), 100, 0xFFFFFF, &frame);

            frame.pixels[0] = 0xFFFFFF;
            
            InvalidateRect(game_hwnd, NULL, false);
            UpdateWindow(game_hwnd);
        }
        if (!is_minimized(&debug)) {
            background(0x000000, &debug);

            for (int j = 0; j < level.sectors.n; j++) {
                wall *cwall = level.sectors.arr[j].list.fwall;
                while (cwall != NULL) {
                    
                    int color = cwall->portal ? 0xFF0000 : 0xFFFFFF;
                    drawline
                    (
                        AddCameraOffset(WorldPosToFramePos(cwall->a, zoom, &debug), cameraOffset), 
                        AddCameraOffset(WorldPosToFramePos(cwall->b, zoom, &debug), cameraOffset), 
                        1, color, &debug
                    ); 
                    
                    if (mouse.dragging) {
                        f32 drag_sensitivity = 0.1;
                        cameraOffset.x -= (mouse.pos.x-mouse.pPos.x)*drag_sensitivity;
                        cameraOffset.y -= (mouse.pos.y-mouse.pPos.y)*drag_sensitivity;
                    }
                    cwall = cwall->next;
                }
            }
            InvalidateRect(debug_hwnd, NULL, false);
            UpdateWindow(debug_hwnd);
            mouse.pPos = mouse.pos;
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
        player.pos.x -= 5;
    }
    if (keyboard['D']) {
        player.pos.x += 5;
    }
    if (keyboard['S']) {
        player.pos.y -= 5;
    }
    if (keyboard['W']) {
        player.pos.y += 5;
    }
    if (keyboard[188]) {
        zoom -= 0.025;
    }
    if (keyboard[190]) {
        zoom += 0.025;
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
        case WM_MOUSEMOVE: {
			mouse.x = LOWORD(lParam);
			mouse.y = debug.height - 1 - HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: mouse.buttons |=  MOUSE_LEFT; mouse.dragging = true; break;
		case WM_LBUTTONUP:   mouse.buttons &= ~MOUSE_LEFT; mouse.dragging = false; break;
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