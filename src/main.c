#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

void printBuffer(struct buffer *buffer, struct buffer_cursor *cursor) {
    struct buffer_cursor cursorCopy;
    buffer_cursor_init_copy(&cursorCopy, cursor);

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

    buffer_insertAtCursor(&buffer, &cursor, "Hel", 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    printBuffer(&buffer, &printCursor);

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

    printf("%d %d\n", buffer.numModifications, buffer.bufferLength);
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

    printf("%d %d\n", buffer.numModifications, buffer.bufferLength);

    buffer_unregisterCursor(&buffer, &cursor);
    buffer_unregisterCursor(&buffer, &printCursor);
    buffer_deinit(&buffer);
    
}
/*
void test1(void) {
    struct buffer buffer;
    buffer_init(&buffer, "Hello world!", 13);

    printChar(&buffer);
    buffer_moveCursor(&buffer, 6);
    printChar(&buffer);
    buffer_moveCursor(&buffer, -3);
    printChar(&buffer);

    printf("\n");

    buffer_setAtCursor(&buffer, 'X');
    buffer_deleteAtCursor(&buffer);
    buffer_deleteAtCursor(&buffer);
    buffer_deleteAtCursor(&buffer);
    buffer_deleteAtCursor(&buffer);
    buffer_deleteAtCursor(&buffer);
    buffer_deleteAtCursor(&buffer);
    
    buffer_moveCursor(&buffer, 1);
    buffer_deleteAtCursor(&buffer);

    buffer_moveCursor(&buffer, -4);
    int i = 6;
    while (--i) {
        printChar(&buffer);
        buffer_moveCursor(&buffer, 1);
    }
    printChar(&buffer);
    buffer_moveCursor(&buffer, 1);

    buffer_deinit(&buffer);
    
}
*/
int main(int argc, char **argv) {
    test2();
    return 0;
}