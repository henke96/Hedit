#if defined(_WIN32)
#include "win/window_impl.h"
#elif defined(__linux__)
#include "linux/window_impl.h"
#else
#error Unsupported platform.
#endif