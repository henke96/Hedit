#include "window/window.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <malloc.h>
#include <stdint.h>

static struct {
    Display *display;
    Window window;
    GC gc;
    XImage *xImage;
    int shouldExit;
    uint32_t *pixels;
} self = {0};

int window_init(
    unsigned width,
    unsigned height,
    const char *title
) {
    self.display = XOpenDisplay(NULL);
    if (self.display == NULL) return 0;

    Window rootWindow = DefaultRootWindow(self.display);

    XVisualInfo xVisualInfo;
    if (!XMatchVisualInfo(
            self.display,
            DefaultScreen(self.display),
            32,
            TrueColor,
            &xVisualInfo
        )
    ) return 0;

    Colormap colormap = XCreateColormap(
        self.display,
        rootWindow,
        xVisualInfo.visual,
        AllocNone
    );

    XSetWindowAttributes xSetWindowAttributes = {
        .colormap = colormap,
        .event_mask = StructureNotifyMask | ExposureMask | KeyPressMask,
        .border_pixel = 0
    };

    self.window = XCreateWindow(
        self.display,
        rootWindow,
        0,
        0,
        width,
        height,
        0,
        xVisualInfo.depth,
        InputOutput,
        xVisualInfo.visual,
        CWColormap | CWEventMask | CWBorderPixel,
        &xSetWindowAttributes
    );

    self.pixels = malloc(width*height*sizeof(*self.pixels)+1000);
    self.pixels[0] = 0xFFFFFFFF;
    self.xImage = XCreateImage(self.display, xVisualInfo.visual, xVisualInfo.depth, ZPixmap, 0, self.pixels, width, height, 32, 0);

    XStoreName(self.display, self.window, title);
    XMapWindow(self.display, self.window);

    self.gc = XCreateGC(self.display, self.window, 0, 0);
    return 1;
}

void window_loop(void) {
    while (!self.shouldExit) {
        XEvent xEvent;
        XNextEvent(self.display, &xEvent);
        XPutImage(self.display, self.window, self.gc, self.xImage, 0, 0, 0, 0, 800, 600);
        XFlush(self.display);
    }
}

void window_exit(void) {

}