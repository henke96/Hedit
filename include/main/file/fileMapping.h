#pragma once

#include <inttypes.h>

#define fileMapping_init_FILE_READING_ERROR -1
#define fileMapping_init_MEMORY_ALLOCATION_ERROR -2

#if defined(HEDIT_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

struct fileMapping {
    int64_t contentSize;
    char *content;

#if defined(HEDIT_PLATFORM_WINDOWS)
    HANDLE hFile;
    HANDLE hFileMapping;
#elif defined(HEDIT_PLATFORM_LINUX)
    int fd;
#endif
};

static int fileMapping_init(struct fileMapping *self, const char *path);
static void fileMapping_deinit(struct fileMapping *self);

