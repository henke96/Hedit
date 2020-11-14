#pragma once
#include <inttypes.h>

struct bufferChunk {
    const char *text;
    int64_t length;
    int64_t cursorOffset;
};
