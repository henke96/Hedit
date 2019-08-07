#include "buffer_test.h"
#include "buffer.h"

#include <stdio.h>
#include <string.h>

static bool assertBufferContents(const struct buffer *buffer, const char *content, const char *testName, int testLine) {
    struct buffer_cursor cursor;
    
    buffer_cursor_init(&cursor, buffer);
    int64_t contentLength = strlen(content);
    int64_t bufferLength = buffer_getLength(buffer);
    if (bufferLength == contentLength) {
        if (bufferLength != 0) {
            int64_t i = 0;
            char c = buffer_getAtCursor(buffer, &cursor);
            while(1) {
                if (c != content[i]) goto fail;
                if (++i == bufferLength) break;
                c = buffer_cursorNext(buffer, &cursor);
            }
        }
        return true;
    }
    fail:
    printTestFail(testName, __FILE__, testLine);
    printf("\tBuffer contents not as expected!\n");
    printf("\tExpected: '%s'\n\tActual: '", content);
    // Print buffer contents.
    if (bufferLength != 0) {
        buffer_cursor_init(&cursor, buffer);
        int64_t i = 0;
        char c = buffer_getAtCursor(buffer, &cursor);
        while(1) {
            printf("%c", c);
            if (++i == bufferLength) break;
            c = buffer_cursorNext(buffer, &cursor);
        }
    }
    printf("'\n");
    return false;
}

struct test_result buffer_test_simple(void) {
    const char *testName = "buffer_test_simple";

    struct buffer buffer;
    const char *text = "Hello world!";

    buffer_init(&buffer, text, strlen(text));

    if (!assertBufferContents(&buffer, "Hello world!", testName, __LINE__)) return test_result_create(false, testName);

    return test_result_create(true, testName);
}
