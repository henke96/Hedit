#include "textBufferView.h"

void textBufferView_init(struct textBufferView *self, const char *text, int64_t textLength) {
    self->bufferView.type = bufferView_type_TEXT;
}

void textBufferView_deinit(struct textBufferView *self) {

}