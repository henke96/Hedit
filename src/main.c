#include "buffer.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void printBuffer(struct buffer *buffer, struct buffer_cursor *cursor) {
    struct buffer_cursor cursorCopy;
    buffer_cursor_initCopy(&cursorCopy, cursor);

    int64_t i = buffer->bufferLength - cursorCopy.bufferOffset;
    if (i) {
        printf("%c", buffer_getAtCursor(buffer, &cursorCopy));
        while (--i) {
            //buffer_moveCursor(buffer, cursor, 1);
            
            printf("%c", buffer_cursorNext(buffer, &cursorCopy));
        }
    }
    printf("\n");
}

void printBufferData(struct buffer *buffer) {
    printf("Printing buffer data:\n\tLen: %" PRId64 ", Orig len: %" PRId64 ", Num modifications: %" PRId32 "\n\tText:\n", buffer->bufferLength, buffer->textLength, buffer->numModifications);
    int32_t nextModificationIndex = 0;
    int64_t textOffset = 0;
    while (1) {
        if (nextModificationIndex < buffer->numModifications) {
            struct buffer_modification *nextModification = &buffer->modifications[nextModificationIndex];
            if (nextModification->intervalStart > textOffset) {
                printf(
                    "\t\t'%.*s'\n",
                    (int)(nextModification->intervalStart - textOffset), &buffer->text[textOffset]
                );
            }
            printf(
                "\t\tModification %" PRId32 ": '%.*s' -> '%.*s'\n",
                nextModificationIndex,
                (int)(nextModification->intervalEnd - nextModification->intervalStart), &buffer->text[nextModification->intervalStart],
                (int)nextModification->insertLength, nextModification->insertEnd - nextModification->insertLength
            );
            ++nextModificationIndex;
            textOffset = nextModification->intervalEnd;
        } else {
            if (buffer->textLength - textOffset > 0) {
                printf(
                    "\t\t'%.*s'\n",
                    (int)(buffer->textLength - textOffset), &buffer->text[textOffset]
                );
            }
            break;
        }
    }
}

void test2(void) {
    struct buffer buffer;
    buffer_init(&buffer, "Hello world!", 12);

    struct buffer_cursor cursor;
    buffer_cursor_init(&cursor, &buffer);

    struct buffer_cursor printCursor;
    buffer_cursor_init(&printCursor, &buffer);

    buffer_registerCursor(&buffer, &cursor);
    buffer_registerCursor(&buffer, &printCursor);


    printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    printBufferData(&buffer);

    buffer_insertAtCursor(&buffer, &cursor, "Hel", 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    printBuffer(&buffer, &printCursor);

    printBufferData(&buffer);

    // Start
    
    buffer_insertAtCursor(&buffer, &cursor, "h", 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_insertAtCursor(&buffer, &cursor, "hej ", 4);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 5);
    buffer_insertAtCursor(&buffer, &cursor, " x ", 3);
    buffer_moveCursor(&buffer, &cursor, -8);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 13);
    buffer_insertAtCursor(&buffer, &cursor, " humble", 7);
    buffer_moveCursor(&buffer, &cursor, -20);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 26);
    buffer_insertAtCursor(&buffer, &cursor, "!", 1);
    buffer_moveCursor(&buffer, &cursor, -27);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 28);
    buffer_insertAtCursor(&buffer, &cursor, " wooo", 5);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "s", 1);
    buffer_moveCursor(&buffer, &cursor, -16);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 19);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 1);
    buffer_deleteAtCursor(&buffer, &cursor, 8);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    printBufferData(&buffer);
    // Start undoing

    buffer_moveCursorTo(&buffer, &cursor, 1);
    buffer_insertAtCursor(&buffer, &cursor, "ej h x H", 8);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "sumble world!! wooo", 19);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 28);
    buffer_deleteAtCursor(&buffer, &cursor, 5);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 26);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 13);
    buffer_deleteAtCursor(&buffer, &cursor, 7);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 5);
    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 4);
    printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 1);
    printBuffer(&buffer, &printCursor);

    printBufferData(&buffer);

    buffer_unregisterCursor(&buffer, &cursor);
    buffer_unregisterCursor(&buffer, &printCursor);
    buffer_deinit(&buffer);
    
}
/*
void testFileMapping(void) {
    char temp[1000];

    GetCurrentDirectoryA(1000, &temp[0]);
    printf("%s\n", temp);

    HANDLE hFile = CreateFileA(
        "test.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("File create error: %lu\n", GetLastError());
        return;
    }

    HANDLE hFileMapping = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL
    );

    if (hFileMapping == NULL) {
        printf("File mapping error: %lu\n", GetLastError());
        return;
    }

    char *content = MapViewOfFile(
        hFileMapping,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    if (content == NULL) {
        printf("File vieww error: %lu\n", GetLastError());
        return;
    }

    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile, &fileSize);
    for (int i = 0; i < fileSize.QuadPart; ++i)
        printf("%c", content[i]);
    printf("\n");

    UnmapViewOfFile(content);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
}*/

int main(int argc, char **argv) {
    //testFileMapping();
    test2();
    return 0;
}
