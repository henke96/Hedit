#include "main/buffer.h"
#include "main/file/fileMapping.h"
#include "main/file/fileWriter.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

static void main_printBuffer(struct buffer *buffer, struct buffer_cursor *cursor) {
    struct buffer_cursor cursorCopy;
    buffer_cursor_init_copy(&cursorCopy, cursor);

    while (buffer_cursor_getOffset(&cursorCopy) != buffer_getLength(buffer)) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &cursorCopy);
        assert(bufferChunk_getCursorOffset(&chunk) == 0);
        assert(bufferChunk_getLength(&chunk) > 0);

        printf("%.*s", (int)bufferChunk_getLength(&chunk), bufferChunk_getText(&chunk));

        buffer_moveCursor(buffer, &cursorCopy, bufferChunk_getLength(&chunk));
    }
    buffer_cursor_deinit(&cursorCopy);
    printf("\n");
}

static void main_printBufferData(struct buffer *buffer) {
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

static int main_writeBufferToFile(const struct buffer *buffer, const char *path) {
    struct fileWriter fileWriter;
    fileWriter_init(&fileWriter, path);
    if (fileWriter_open(&fileWriter) < 0) {
        return -1;
    }

    struct buffer_cursor readCursor;
    buffer_cursor_init(&readCursor, buffer);

    while (buffer_cursor_getOffset(&readCursor) != buffer_getLength(buffer)) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &readCursor);
        assert(bufferChunk_getCursorOffset(&chunk) == 0);
        assert(bufferChunk_getLength(&chunk) > 0);
        if (fileWriter_append(&fileWriter, bufferChunk_getText(&chunk), bufferChunk_getLength(&chunk)) < 0) {
            return -1;
        }
        buffer_moveCursor(buffer, &readCursor, bufferChunk_getLength(&chunk));
    }
    buffer_cursor_deinit(&readCursor);

    if (fileWriter_close(&fileWriter) < 0) {
        return -1;
    }
    fileWriter_deinit(&fileWriter);
    return 0;
}

static void main_test2(void) {
    struct fileMapping fileMapping;
    if (fileMapping_init(&fileMapping, "test.txt") < 0) {
        return;
    }

    struct buffer buffer;
    buffer_init(&buffer, fileMapping_getContent(&fileMapping), fileMapping_getContentSize(&fileMapping));

    struct buffer_cursor cursor;
    buffer_cursor_init(&cursor, &buffer);

    struct buffer_cursor printCursor;
    buffer_cursor_init(&printCursor, &buffer);

    buffer_registerCursor(&buffer, &cursor);
    buffer_registerCursor(&buffer, &printCursor);


    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    buffer_insertAtCursor(&buffer, &cursor, "Hel", 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    // Start

    buffer_insertAtCursor(&buffer, &cursor, "h", 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_insertAtCursor(&buffer, &cursor, "hej ", 4);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    buffer_moveCursorTo(&buffer, &printCursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 5);
    buffer_insertAtCursor(&buffer, &cursor, " x ", 3);
    buffer_moveCursor(&buffer, &cursor, -8);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 13);
    buffer_insertAtCursor(&buffer, &cursor, " humble", 7);
    buffer_moveCursor(&buffer, &cursor, -20);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 26);
    buffer_insertAtCursor(&buffer, &cursor, "!", 1);
    buffer_moveCursor(&buffer, &cursor, -27);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursor(&buffer, &cursor, 28);
    buffer_insertAtCursor(&buffer, &cursor, " wooo", 5);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "s", 1);
    buffer_moveCursor(&buffer, &cursor, -16);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 19);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 1);
    buffer_deleteAtCursor(&buffer, &cursor, 8);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);
    // Start undoing

    buffer_moveCursorTo(&buffer, &cursor, 1);
    buffer_insertAtCursor(&buffer, &cursor, "ej h x H", 8);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "sumble world!! wooo", 19);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 28);
    buffer_deleteAtCursor(&buffer, &cursor, 5);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 26);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 13);
    buffer_deleteAtCursor(&buffer, &cursor, 7);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_moveCursorTo(&buffer, &cursor, 5);
    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_moveCursorTo(&buffer, &cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 4);
    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 1);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    buffer_unregisterCursor(&buffer, &cursor);
    buffer_unregisterCursor(&buffer, &printCursor);

    main_writeBufferToFile(&buffer, ".out.txt");

    buffer_cursor_deinit(&cursor);
    buffer_cursor_deinit(&printCursor);

    buffer_deinit(&buffer);
    fileMapping_deinit(&fileMapping);
}

static void main_simple(void) {
    struct fileMapping fileMapping;
    if (fileMapping_init(&fileMapping, "Configuration.mk") < 0) {
        return;
    }

    struct buffer buffer;
    buffer_init(&buffer, fileMapping_getContent(&fileMapping), fileMapping_getContentSize(&fileMapping));

    struct buffer_cursor lineCursor;
    buffer_cursor_init(&lineCursor, &buffer);
    buffer_registerCursor(&buffer, &lineCursor);

    while (1) {
        struct buffer_cursor cursorCopy;

        // Print a line.
        if (buffer_cursor_getOffset(&lineCursor) == buffer_getLength(&buffer)) goto skipPrintLine;

        if (buffer_getAtCursor(&buffer, &lineCursor) == 10) {
            buffer_moveCursor(&buffer, &lineCursor, 1);
        } else if (buffer_cursor_getOffset(&lineCursor) != 0) goto skipPrintLine;

        buffer_cursor_init_copy(&cursorCopy, &lineCursor);
        while (1) {
            struct bufferChunk chunk = buffer_getCursorChunk(&buffer, &cursorCopy);
            assert(bufferChunk_getLength(&chunk) > 0);

            const char *chunkText = bufferChunk_getText(&chunk);
            int64_t chunkLength = bufferChunk_getLength(&chunk);
            int64_t chunkCursorOffset = bufferChunk_getCursorOffset(&chunk);
            for (int64_t i = chunkCursorOffset; i < chunkLength; ++i) {
                if (chunkText[i] != 10) continue;

                buffer_moveCursor(&buffer, &cursorCopy, i - chunkCursorOffset);
                printf("%d:%.*s", (int)(buffer_cursor_getOffset(&lineCursor)), (int)(i - chunkCursorOffset), &chunkText[chunkCursorOffset]);
                goto foundLineEnd;
            }
            // Line end not in chunk.
            printf("%d:%.*s", (int)(buffer_cursor_getOffset(&lineCursor)), (int)(chunkLength - chunkCursorOffset), &chunkText[chunkCursorOffset]);
            buffer_moveCursor(&buffer, &cursorCopy, (chunkLength - chunkCursorOffset));
            if (buffer_cursor_getOffset(&cursorCopy) == buffer_getLength(&buffer)) break;
        }
        foundLineEnd:
        buffer_cursor_deinit(&lineCursor);
        buffer_cursor_init_copy(&lineCursor, &cursorCopy);
        buffer_cursor_deinit(&cursorCopy);

        skipPrintLine:
        while (getchar() != 10);
    }
}

int main(void) {
    main_test2();
    main_simple();
    return 0;
}
