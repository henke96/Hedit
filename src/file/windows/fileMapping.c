#include "file/fileMapping.h"

#include <stdio.h>

int fileMapping_init(struct fileMapping *self, const char *path) {
    char temp[1000];

    GetCurrentDirectoryA(1000, &temp[0]);
    printf("%s\n", temp);

    self->hFile = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (self->hFile == INVALID_HANDLE_VALUE) {
        printf("File create error: %lu\n", GetLastError());
        goto cleanup_0;
    }

    self->hFileMapping = CreateFileMappingA(
        self->hFile,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL
    );
    if (self->hFileMapping == NULL) {
        printf("File mapping error: %lu\n", GetLastError());
        goto cleanup_1;
    }

    self->content = MapViewOfFile(
        self->hFileMapping,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    if (self->content == NULL) {
        printf("File view error: %lu\n", GetLastError());
        goto cleanup_2;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(self->hFile, &fileSize)) {
        printf("File size error: %lu\n", GetLastError());
        goto cleanup_3;
    }

    self->contentSize = fileSize.QuadPart;

    for (int i = 0; i < self->contentSize; ++i) {
        printf("%c", self->content[i]);
    }
    printf("\n");
    return 0;

    cleanup_3:
    UnmapViewOfFile(self->content);
    cleanup_2:
    CloseHandle(self->hFileMapping);
    cleanup_1:
    CloseHandle(self->hFile);
    cleanup_0:
    return fileMapping_init_FILE_READING_ERROR;
}

void fileMapping_deinit(struct fileMapping *self) {
    UnmapViewOfFile(self->content);
    CloseHandle(self->hFileMapping);
    CloseHandle(self->hFile);
}
