#include "main/buffer.h"
#include "main/file/fileMapping.h"
#include "main/file/fileWriter.h"
#include "main/ui.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

static void main_printBuffer(struct buffer *buffer, struct buffer_cursor *cursor) {
    struct buffer_cursor cursorCopy;
    buffer_cursor_INIT_COPY(cursorCopy, *cursor);

    while (cursorCopy.bufferOffset != buffer->length) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &cursorCopy);
        assert(chunk.cursorOffset == 0);
        assert(chunk.length > 0);

        printf("%.*s", (int)chunk.length, chunk.text);

        buffer_moveCursor(buffer, &cursorCopy, chunk.length);
    }
    buffer_cursor_DEINIT(cursorCopy);
    printf("\n");
}

static void main_printBufferData(struct buffer *buffer) {
    printf("Printing buffer data:\n\tLen: %" PRId64 ", Orig len: %" PRId64 ", Num modifications: %" PRId32 "\n\tText:\n", buffer->length, buffer->textLength, buffer->numModifications);
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
    fileWriter_INIT(fileWriter, path);
    if (fileWriter_open(&fileWriter) < 0) {
        return -1;
    }

    struct buffer_cursor readCursor;
    buffer_cursor_init(&readCursor, buffer);

    while (readCursor.bufferOffset != buffer->length) {
        struct bufferChunk chunk = buffer_getCursorChunk(buffer, &readCursor);
        assert(chunk.cursorOffset == 0);
        assert(chunk.length > 0);
        if (fileWriter_append(&fileWriter, chunk.text, chunk.length) < 0) {
            return -1;
        }
        buffer_moveCursor(buffer, &readCursor, chunk.length);
    }
    buffer_cursor_DEINIT(readCursor);

    if (fileWriter_close(&fileWriter) < 0) {
        return -1;
    }
    fileWriter_DEINIT(fileWriter);
    return 0;
}

static void main_test2(void) {
    struct fileMapping fileMapping;
    if (fileMapping_init(&fileMapping, "test.txt") < 0) {
        return;
    }

    struct buffer buffer;
    buffer_init(&buffer, fileMapping.content, fileMapping.contentSize);

    struct buffer_cursor cursor;
    buffer_cursor_init(&cursor, &buffer);

    struct buffer_cursor printCursor;
    buffer_cursor_init(&printCursor, &buffer);

    buffer_registerCursor(&buffer, &cursor);
    buffer_registerCursor(&buffer, &printCursor);


    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    buffer_insertAtCursor(&buffer, &cursor, "Hel", 3);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    buffer_MOVE_CURSOR_TO(buffer, printCursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    // Start

    buffer_insertAtCursor(&buffer, &cursor, "h", 1);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    buffer_MOVE_CURSOR_TO(buffer, printCursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_insertAtCursor(&buffer, &cursor, "hej ", 4);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    buffer_MOVE_CURSOR_TO(buffer, printCursor, 0);
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
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "s", 1);
    buffer_moveCursor(&buffer, &cursor, -16);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 19);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 1);
    buffer_deleteAtCursor(&buffer, &cursor, 8);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);
    // Start undoing

    buffer_MOVE_CURSOR_TO(buffer, cursor, 1);
    buffer_insertAtCursor(&buffer, &cursor, "ej h x H", 8);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 15);
    buffer_insertAtCursor(&buffer, &cursor, "sumble world!! wooo", 19);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 15);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 28);
    buffer_deleteAtCursor(&buffer, &cursor, 5);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 26);
    buffer_deleteAtCursor(&buffer, &cursor, 1);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 13);
    buffer_deleteAtCursor(&buffer, &cursor, 7);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_MOVE_CURSOR_TO(buffer, cursor, 5);
    buffer_deleteAtCursor(&buffer, &cursor, 3);
    buffer_MOVE_CURSOR_TO(buffer, cursor, 0);
    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 4);
    main_printBuffer(&buffer, &printCursor);

    buffer_deleteAtCursor(&buffer, &cursor, 1);
    main_printBuffer(&buffer, &printCursor);

    main_printBufferData(&buffer);

    buffer_unregisterCursor(&buffer, &cursor);
    buffer_unregisterCursor(&buffer, &printCursor);

    main_writeBufferToFile(&buffer, ".out.txt");

    buffer_cursor_DEINIT(cursor);
    buffer_cursor_DEINIT(printCursor);

    buffer_deinit(&buffer);
    fileMapping_deinit(&fileMapping);
}

static int main_simple(void) {
    struct fileMapping fileMapping;
    if (fileMapping_init(&fileMapping, "src/main/buffer.c") < 0) return 1;

    struct buffer buffer;
    buffer_init(&buffer, fileMapping.content, fileMapping.contentSize);

    struct buffer_cursor lineCursor;
    buffer_cursor_init(&lineCursor, &buffer);
    buffer_registerCursor(&buffer, &lineCursor);

    while (1) {
        struct buffer_cursor cursorCopy;

        // Print a line.
        if (lineCursor.bufferOffset == buffer.length) goto skipPrintLine;

        if (buffer_getAtCursor(&buffer, &lineCursor) == 10) {
            buffer_moveCursor(&buffer, &lineCursor, 1);
        } else if (lineCursor.bufferOffset != 0) goto skipPrintLine;

        buffer_cursor_INIT_COPY(cursorCopy, lineCursor);
        while (1) {
            struct bufferChunk chunk = buffer_getCursorChunk(&buffer, &cursorCopy);
            assert(chunk.length > 0);

            for (int64_t i = chunk.cursorOffset; i < chunk.length; ++i) {
                if (chunk.text[i] != 10) continue;

                buffer_moveCursor(&buffer, &cursorCopy, i - chunk.cursorOffset);
                printf("%d:%.*s", (int)(lineCursor.bufferOffset), (int)(i - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]);
                goto foundLineEnd;
            }
            // Line end not in chunk.
            printf("%d:%.*s", (int)(lineCursor.bufferOffset), (int)(chunk.length - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]);
            buffer_moveCursor(&buffer, &cursorCopy, (chunk.length - chunk.cursorOffset));
            if (cursorCopy.bufferOffset == buffer.length) break;
        }
        foundLineEnd:
        buffer_cursor_DEINIT(lineCursor);
        buffer_cursor_INIT_COPY(lineCursor, cursorCopy);
        buffer_cursor_DEINIT(cursorCopy);

        skipPrintLine:
        while (getchar() != 10);
    }
    return 0;
}

int main(void) {
    struct fileMapping fileMapping;
    if (fileMapping_init(&fileMapping, "src/main/buffer.c") < 0) return 1;

    struct buffer buffer;
    buffer_init(&buffer, fileMapping.content, fileMapping.contentSize);

    struct ui ui;
    ui_init(&ui, &buffer);
    return ui_run(&ui);
}
