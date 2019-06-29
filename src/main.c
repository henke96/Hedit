#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

void printBuffer(struct buffer *buffer) {
    for (int64_t i = 0; i < buffer->bufferLength; ++i) {
        printf("%c", buffer_getAtCursor(buffer));
        buffer_moveCursor(buffer, 1);
        //printf("%d %d\n", i, buffer->cursor.offset);
    }
    buffer_moveCursor(buffer, -buffer->bufferLength);
    printf("\n");
}

void test2(void) {
    struct buffer buffer;
    buffer_init(&buffer, "Hello world!", 12);
    printBuffer(&buffer);
    
    buffer_insertAtCursor(&buffer, "h", 1);
    printBuffer(&buffer);

    buffer_insertAtCursor(&buffer, "hej ", 4);
    printBuffer(&buffer);

    buffer_moveCursor(&buffer, 5);
    buffer_insertAtCursor(&buffer, " x ", 3);
    buffer_moveCursor(&buffer, -5);
    printBuffer(&buffer);

    buffer_moveCursor(&buffer, 13);
    buffer_insertAtCursor(&buffer, " humble", 7);
    buffer_moveCursor(&buffer, -13);
    printBuffer(&buffer);

    buffer_moveCursor(&buffer, 26);
    buffer_insertAtCursor(&buffer, "!", 1);
    buffer_moveCursor(&buffer, -26);
    printBuffer(&buffer);

    buffer_moveCursor(&buffer, 28);
    buffer_insertAtCursor(&buffer, " wooo", 5);
    buffer_moveCursor(&buffer, -28);
    printBuffer(&buffer);

    printf("%d %d\n", buffer.numModifications, buffer.bufferLength);

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