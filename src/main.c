#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

void printChar(struct buffer *buffer) {
    printf("%c", buffer_getAtCursor(buffer));
}

void test2(void) {
    struct buffer buffer;
    buffer_init(&buffer, "Hello world!", 13);
    // Hello hworld!
    buffer_moveCursor(&buffer, 0);
    printChar(&buffer);
    buffer_moveCursor(&buffer, 6);
    printChar(&buffer);
    buffer_moveCursor(&buffer, -3);
    printChar(&buffer);

    buffer_moveCursor(&buffer, 1);
    printChar(&buffer);
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