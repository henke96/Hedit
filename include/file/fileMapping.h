#pragma once

#include <inttypes.h>

#define fileMapping_init_FILE_READING_ERROR -1
#define fileMapping_init_MEMORY_ALLOCATION_ERROR -2

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

struct fileMapping {
    int64_t contentSize;
    char *content;

#if defined(_WIN32)
    HANDLE hFile;
    HANDLE hFileMapping;
#elif defined(__linux__)
    int fd;
#endif
};

int fileMapping_init(struct fileMapping *self, const char *path);
void fileMapping_deinit(struct fileMapping *self);

static inline char *fileMapping_getContent(const struct fileMapping *self) {
    return self->content;
}

static inline int64_t fileMapping_getContentSize(const struct fileMapping *self) {
    return self->contentSize;
}
