#include "runtime_stdio.h"

#define stdout (&__files._stdout)

extern int __put_char(int c, FILE *stream);

int puts(const char *str)
{
    int c;
    int result;
    FILE *stream;

    stream = stdout;
    while ((c = *str++) != 0) {
        result = (fwide(stream, -1) >= 0) ? -1 : ((stream->buffer_length-- != 0) ? (unsigned char)(*stream->buffer_ptr++ = c) : __put_char(c, stream));
        if (result == -1) {
            return -1;
        }
    }
    result = (fwide(stream, -1) >= 0) ? -1 : ((stream->buffer_length-- != 0) ? (unsigned char)(*stream->buffer_ptr++ = '\n') : __put_char('\n', stream));
    if (result == -1) {
        return -1;
    }
    return 0;
}
