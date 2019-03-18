#include "window/window.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_STYLE WS_VISIBLE | WS_OVERLAPPEDWINDOW
#define WINDOW_STYLE_EX WS_EX_LEFT

struct {
    HWND hWnd;
    HINSTANCE hInstance;
    HDC mainHDc;
    HDC bufferHDc;
    unsigned width;
    unsigned height;
    uint32_t *pixels;
    int shouldExit;
} self = {0};

static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int window_init(
    unsigned width,
    unsigned height,
    const char *title
) {
    self.width = width;
    self.height = height;
    LPCSTR className = "hedit";
    self.hInstance = GetModuleHandleA(NULL);

    WNDCLASSA wndClass = {
        .lpfnWndProc = wndProc,
        .hInstance = self.hInstance,
        .lpszClassName = className,
        .hCursor = LoadCursorA(NULL, IDC_ARROW)
    };
    if (!RegisterClassA(&wndClass)) return 0;

    RECT windowRect = {
        .left = 0,
        .right = width,
        .top = 0,
        .bottom = height
    };
    if (!AdjustWindowRectEx(&windowRect, WINDOW_STYLE, FALSE, WINDOW_STYLE_EX)) return 0;

    self.hWnd = CreateWindowExA(
        WINDOW_STYLE_EX,
        className,
        title,
        WINDOW_STYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (windowRect.right - windowRect.left),
        (windowRect.bottom - windowRect.top),
        NULL,
        NULL,
        self.hInstance,
        NULL
    );
    return self.hWnd != NULL;
}

void window_loop(void) {
    MSG msg;
    while (!self.shouldExit) {
        if (!GetMessageA(&msg, NULL, 0, 0)) break;
        DispatchMessageA(&msg);
    }
}

void window_exit(void) {
    self.shouldExit = 1;
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            self.mainHDc = GetDC(hWnd);
            
            BITMAPINFO bmi = {
                .bmiHeader = {
                    .biSize = sizeof(BITMAPINFOHEADER),
                    .biWidth = self.width,
                    .biHeight = self.height,
                    .biPlanes = 1,
                    .biBitCount = 32,
                    .biCompression = BI_RGB
                }
            };
            HBITMAP buffer = CreateDIBSection(
                self.mainHDc,
                &bmi,
                DIB_RGB_COLORS,
                &self.pixels,
                NULL,
                0
            );
            self.bufferHDc = CreateCompatibleDC(self.mainHDc);
            SelectObject(self.bufferHDc, buffer);

            for (int i = 0; i < self.width*self.height; ++i) {
                self.pixels[i] = i % 50;
            }
            break;
        }
        case WM_CLOSE: {
            PostQuitMessage(0);
            break;
        }
        case WM_ERASEBKGND: {
            return 1;
        }
        case WM_PAINT: {
            RECT updateRect;
            if (GetUpdateRect(hWnd, &updateRect, FALSE)) {
                printf("%d %d %d %d\n", updateRect.left, updateRect.top, updateRect.right, updateRect.bottom);
                BitBlt(self.mainHDc, updateRect.left, updateRect.top, updateRect.right - updateRect.left, updateRect.bottom - updateRect.top, self.bufferHDc, updateRect.left, updateRect.top, SRCCOPY);
                ValidateRect(hWnd, &updateRect);
            }
            break;
        }
        case WM_WINDOWPOSCHANGED: {    
            break;
        }
        case WM_LBUTTONDOWN: {          
            break;
        }
        case WM_RBUTTONDOWN: {           
            break;
        }
        case WM_LBUTTONUP: {
            break;
        }
        case WM_RBUTTONUP: {        
            break;
        }
        case WM_MOUSEMOVE: {
            break;
        }
        case WM_KEYDOWN: {
            break;
        }
        default: {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}