#include "test/test_buffer.h"
#include "main/buffer.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool assertBufferContents(const struct buffer *buffer, const char *content, const char *testName, int32_t testLine) {
    struct buffer_cursor cursor;
    
    buffer_cursor_init(&cursor, buffer);
    int64_t contentLength = strlen(content);
    int64_t bufferLength = buffer_getLength(buffer);
    if (bufferLength == contentLength) {
        for (int64_t i = 0; i < bufferLength; ++i) {
            if (buffer_getAtCursor(buffer, &cursor) != content[i]) goto fail;
            buffer_moveCursor(buffer, &cursor, 1);
        }
        return true;
    }
    fail:
    printTestFail(testName, __FILE__, testLine);
    printf(
        "\tBuffer contents not as expected!\n"
        "\tExpected: '%s'\n\tActual: '",
        content
    );
    // Print buffer contents.
    while (buffer_cursor_getOffset(&cursor) != bufferLength) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &cursor);
        printf("%.*s", (int)bufferChunk_getLength(&chunk), bufferChunk_getText(&chunk));
        buffer_moveCursor(buffer, &cursor, bufferChunk_getLength(&chunk));
    }
    printf("'\n");
    return false;
}

static bool assertCursorOffset(const struct buffer_cursor *cursor, int64_t offset, const char *testName, int32_t testLine) {
    if (buffer_cursor_getOffset(cursor) == offset) return true;
    printTestFail(testName, __FILE__, testLine);
    printf(
        "\tCursor offset not as expected!\n"
        "\tExpected: %" PRId64 "\n"
        "\tActual: %" PRId64 "\n",
        offset, buffer_cursor_getOffset(cursor)
    );
    return false;
}

struct test_result test_buffer_simple(void) {
    const char *testName = "buffer_simple";

    struct buffer buffer;
    struct buffer_cursor cursor;

    const char *tempText = "Hello world!";
    buffer_init(&buffer, tempText, strlen(tempText));
    if (!assertBufferContents(&buffer, "Hello world!", testName, __LINE__)) return test_result_create(false, testName);

    buffer_cursor_init(&cursor, &buffer);
    buffer_registerCursor(&buffer, &cursor);

    buffer_moveCursor(&buffer, &cursor, 5);
    buffer_deleteAtCursor(&buffer, &cursor, 6);
    if (!assertBufferContents(&buffer, "Hello!", testName, __LINE__)) return test_result_create(false, testName);
    if (!assertCursorOffset(&cursor, 5, testName, __LINE__)) return test_result_create(false, testName);

    tempText = " handsome";
    buffer_insertAtCursor(&buffer, &cursor, tempText, strlen(tempText));
    if (!assertBufferContents(&buffer, "Hello handsome!", testName, __LINE__)) return test_result_create(false, testName);
    if (!assertCursorOffset(&cursor, 14, testName, __LINE__)) return test_result_create(false, testName);

    buffer_moveCursorTo(&buffer, &cursor, 0);
    tempText = "Ahoy ";
    buffer_insertAtCursor(&buffer, &cursor, tempText, strlen(tempText));
    if (!assertBufferContents(&buffer, "Ahoy Hello handsome!", testName, __LINE__)) return test_result_create(false, testName);
    if (!assertCursorOffset(&cursor, 5, testName, __LINE__)) return test_result_create(false, testName);

    buffer_deleteAtCursor(&buffer, &cursor, 6);
    if (!assertBufferContents(&buffer, "Ahoy handsome!", testName, __LINE__)) return test_result_create(false, testName);
    if (!assertCursorOffset(&cursor, 5, testName, __LINE__)) return test_result_create(false, testName);

    return test_result_create(true, testName);
}

struct test_result test_buffer_simpleMultiCursor(void) {
    const char *testName = "buffer_simpleMultiCursor";

    #define simpleMultiCursor_NUM_CURSORS 5

    struct buffer buffer;
    const char *tempText = (
        "Hello world!\n"
        "Hello world!\n"
        "Hello world!\n"
        "Hello world!\n"
        "Hello world!\n"
    );
    buffer_init(&buffer, tempText, strlen(tempText));
    if (!assertBufferContents(&buffer, tempText, testName, __LINE__)) return test_result_create(false, testName);

    int64_t currentCursorOffset = 0;
    struct buffer_cursor cursors[simpleMultiCursor_NUM_CURSORS];
    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_cursor_init(&cursors[i], &buffer);
        buffer_registerCursor(&buffer, &cursors[i]);
        buffer_moveCursorTo(&buffer, &cursors[i], currentCursorOffset);
        currentCursorOffset += 13;
    }

    currentCursorOffset = 0;
    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_deleteAtCursor(&buffer, &cursors[i], 6);
        if (!assertCursorOffset(&cursors[i], currentCursorOffset, testName, __LINE__)) return test_result_create(false, testName);
        currentCursorOffset += 7;
    }

    if (
        !assertBufferContents(
            &buffer,
            "world!\n"
            "world!\n"
            "world!\n"
            "world!\n"
            "world!\n",
            testName,
            __LINE__
        )
    ) return test_result_create(false, testName);

    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_moveCursor(&buffer, &cursors[i], 5);
        tempText = " tour";
        buffer_insertAtCursor(&buffer, &cursors[i], tempText, strlen(tempText));
    }

    if (
        !assertBufferContents(
            &buffer,
            "world tour!\n"
            "world tour!\n"
            "world tour!\n"
            "world tour!\n"
            "world tour!\n",
            testName,
            __LINE__
        )
    ) return test_result_create(false, testName);

    {
        int i = simpleMultiCursor_NUM_CURSORS - 1;
        buffer_deleteAtCursor(&buffer, &cursors[i], 2);
        buffer_moveCursor(&buffer, &cursors[i], -12);
        buffer_deleteAtCursor(&buffer, &cursors[i], 3);
        buffer_insertAtCursor(&buffer, &cursors[i], "W", 1);

        while (--i) {
            buffer_moveCursor(&buffer, &cursors[i], -13);
            buffer_deleteAtCursor(&buffer, &cursors[i], 3);
            buffer_insertAtCursor(&buffer, &cursors[i], "W", 1);
        }
        // i == 0
        buffer_moveCursor(&buffer, &cursors[i], -11);
        buffer_deleteAtCursor(&buffer, &cursors[i], 1);
        buffer_insertAtCursor(&buffer, &cursors[i], "W", 1);
    }

    if (
        !assertBufferContents(
            &buffer,
            "World tour"
            "World tour"
            "World tour"
            "World tour"
            "World tour",
            testName,
            __LINE__
        )
    ) return test_result_create(false, testName);

    return test_result_create(true, testName);
}
