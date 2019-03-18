#include "window/window.h"
#include <stdlib.h>
int main(int argc, char **argv) {
    if (!window_init(800, 600, "home/henrik/CLionProjects/Hedit/src/main.c | Ctrl+h for help")) return EXIT_FAILURE;

    window_loop();
    return 0;
}