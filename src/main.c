#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

void printChar(struct buffer *buffer) {
    printf("%c", buffer_getAtCursor(buffer));
}

int main(int argc, char **argv) {
    struct buffer buffer;
    if (buffer_init(&buffer, "Hello world!", 13) < 0) {
        printf("Failed to init buffer.\n");
        return 1;
    }

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
    return 0;
}