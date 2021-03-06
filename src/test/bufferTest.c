#include "test/bufferTest.h"
#include "main/buffer.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool bufferTest_assertBufferContents(const struct buffer *buffer, const char *content, const char *testName, int32_t testLine) {
    struct buffer_cursor cursor;
    
    buffer_cursor_init(&cursor, buffer);
    int64_t contentLength = strlen(content);
    int64_t bufferLength = buffer->length;
    if (bufferLength == contentLength) {
        for (int64_t i = 0; i < bufferLength; ++i) {
            if (buffer_getAtCursor(buffer, &cursor) != content[i]) goto fail;
            buffer_moveCursor(buffer, &cursor, 1);
        }
        return true;
    }
    fail:
    test_printTestFail(testName, __FILE__, testLine);
    printf(
        "\tBuffer contents not as expected!\n"
        "\tExpected: '%s'\n\tActual: '",
        content
    );
    // Print buffer contents.
    while (cursor.bufferOffset != bufferLength) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &cursor);
        printf("%.*s", (int)chunk.length, chunk.text);
        buffer_moveCursor(buffer, &cursor, chunk.length);
    }
    buffer_cursor_DEINIT(cursor);
    printf("'\n");
    return false;
}

static bool bufferTest_assertCursorOffset(const struct buffer_cursor *cursor, int64_t offset, const char *testName, int32_t testLine) {
    if (cursor->bufferOffset == offset) return true;
    test_printTestFail(testName, __FILE__, testLine);
    printf(
        "\tCursor offset not as expected!\n"
        "\tExpected: %" PRId64 "\n"
        "\tActual: %" PRId64 "\n",
        offset, cursor->bufferOffset
    );
    return false;
}

static struct test_result bufferTest_simple(void) {
    const char *testName = "buffer_test_simple";

    struct buffer buffer;
    struct buffer_cursor cursor;

    const char *tempText = "Hello world!";
    buffer_init(&buffer, tempText, strlen(tempText));
    if (!bufferTest_assertBufferContents(&buffer, "Hello world!", testName, __LINE__)) goto fail;

    buffer_cursor_init(&cursor, &buffer);
    buffer_registerCursor(&buffer, &cursor);

    buffer_moveCursor(&buffer, &cursor, 5);
    buffer_deleteAtCursor(&buffer, &cursor, 6);
    if (!bufferTest_assertBufferContents(&buffer, "Hello!", testName, __LINE__)) goto fail;
    if (!bufferTest_assertCursorOffset(&cursor, 5, testName, __LINE__)) goto fail;

    tempText = " handsome";
    buffer_insertAtCursor(&buffer, &cursor, tempText, strlen(tempText));
    if (!bufferTest_assertBufferContents(&buffer, "Hello handsome!", testName, __LINE__)) goto fail;
    if (!bufferTest_assertCursorOffset(&cursor, 14, testName, __LINE__)) goto fail;

    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    tempText = "Ahoy ";
    buffer_insertAtCursor(&buffer, &cursor, tempText, strlen(tempText));
    if (!bufferTest_assertBufferContents(&buffer, "Ahoy Hello handsome!", testName, __LINE__)) goto fail;
    if (!bufferTest_assertCursorOffset(&cursor, 5, testName, __LINE__)) goto fail;

    buffer_deleteAtCursor(&buffer, &cursor, 6);
    if (!bufferTest_assertBufferContents(&buffer, "Ahoy handsome!", testName, __LINE__)) goto fail;
    if (!bufferTest_assertCursorOffset(&cursor, 5, testName, __LINE__)) goto fail;

    bool success = true;
    goto cleanup;

    fail:
    success = false;

    cleanup:
    buffer_cursor_DEINIT(cursor);
    buffer_deinit(&buffer);
    return test_result_create(success, testName);
}

struct test_result bufferTest_simpleMultiCursor(void) {
    const char *testName = "bufferTest_simpleMultiCursor";

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
    if (!bufferTest_assertBufferContents(&buffer, tempText, testName, __LINE__)) goto fail;

    int64_t currentCursorOffset = 0;
    struct buffer_cursor cursors[simpleMultiCursor_NUM_CURSORS];
    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_cursor_init(&cursors[i], &buffer);
        buffer_registerCursor(&buffer, &cursors[i]);
        buffer_MOVE_CURSOR_TO(buffer, cursors[i], currentCursorOffset);
        currentCursorOffset += 13;
    }

    currentCursorOffset = 0;
    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_deleteAtCursor(&buffer, &cursors[i], 6);
        if (!bufferTest_assertCursorOffset(&cursors[i], currentCursorOffset, testName, __LINE__)) goto fail;
        currentCursorOffset += 7;
    }

    if (
        !bufferTest_assertBufferContents(
            &buffer,
            "world!\n"
            "world!\n"
            "world!\n"
            "world!\n"
            "world!\n",
            testName,
            __LINE__
        )
    ) goto fail;

    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_moveCursor(&buffer, &cursors[i], 5);
        tempText = " tour";
        buffer_insertAtCursor(&buffer, &cursors[i], tempText, strlen(tempText));
    }

    if (
        !bufferTest_assertBufferContents(
            &buffer,
            "world tour!\n"
            "world tour!\n"
            "world tour!\n"
            "world tour!\n"
            "world tour!\n",
            testName,
            __LINE__
        )
    ) goto fail;

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
        !bufferTest_assertBufferContents(
            &buffer,
            "World tour"
            "World tour"
            "World tour"
            "World tour"
            "World tour",
            testName,
            __LINE__
        )
    ) goto fail;

    bool success = true;
    goto cleanup;

    fail:
    success = false;

    cleanup:
    for (int i = 0; i < simpleMultiCursor_NUM_CURSORS; ++i) {
        buffer_cursor_DEINIT(cursors[i]);
    }
    buffer_deinit(&buffer);
    return test_result_create(success, testName);
}
