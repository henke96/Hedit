#pragma once
#include <inttypes.h>

struct bufferChunk {
    const char *text;
    int64_t length;
    int64_t cursorOffset;
};

#define bufferChunk_GET_TEXT(SELF) ((SELF).text)
#define bufferChunk_GET_LENGTH(SELF) ((SELF).length)
#define bufferChunk_GET_CURSOR_OFFSET(SELF) ((SELF).cursorOffset)