#include "runtime_stdio.h"

int __put_char(int c, FILE *stream)
{
    int kind = stream->file_mode.file_kind;

    stream->buffer_length = 0;
    if (stream->file_state.error || kind == __closed_file)
        return -1;

    if (kind == __console_file)
        __stdio_atexit();

    if (stream->file_state.io_state == __neutral) {
        if (stream->file_mode.io_mode & __write) {
            stream->file_state.io_state = __writing;
            __prep_buffer(stream);
        }
    }

    if (stream->file_state.io_state != __writing) {
        set_error(stream);
        return -1;
    }

    if (stream->file_mode.buffer_mode == _IOFBF ||
        stream->buffer_size == stream->buffer_ptr - stream->buffer) {
        if (__flush_buffer(stream, 0)) {
            set_error(stream);
            return -1;
        }
    }

    stream->buffer_length--;
    *stream->buffer_ptr++ = c;

    if (stream->file_mode.buffer_mode != _IOFBF) {
        if (stream->file_mode.buffer_mode == _IONBF || c == '\n') {
            if (__flush_buffer(stream, 0)) {
                set_error(stream);
                return -1;
            }
        }
        stream->buffer_length = 0;
    }

    return (unsigned char)c;
}
