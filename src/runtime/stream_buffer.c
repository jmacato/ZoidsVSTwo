/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common/buffer_io.c.
 */
#include "runtime_stdio.h"
#define NULL 0
void __prep_buffer(FILE *);
int __flush_buffer(FILE *, size_t *);
int fwide(FILE *, int);
void __stdio_atexit(void);

void __prep_buffer(FILE *file)
{
    file->buffer_ptr = file->buffer;
    file->buffer_length = file->buffer_size;
    file->buffer_length -= file->position & file->buffer_alignment;
    file->buffer_position = file->position;
}

static inline void __convert_from_newlines(unsigned char *buffer, size_t *count)
{
    /* This runtime leaves newline conversion disabled. */
}
int __flush_buffer(FILE *file, size_t *bytes_flushed)
{
    size_t buffer_len;
    int ioresult;

    buffer_len = file->buffer_ptr - file->buffer;

    if (buffer_len) {
        file->buffer_length = buffer_len;

        if (!file->file_mode.binary_io)
            __convert_from_newlines(file->buffer, &file->buffer_length);

        ioresult = (*file->write_fn)(file->handle, file->buffer, &file->buffer_length,
                                     file->idle_fn);

        if (bytes_flushed)
            *bytes_flushed = file->buffer_length;

        if (ioresult)
            return ioresult;

        file->position += file->buffer_length;
    }

    __prep_buffer(file);

    return __no_io_error;
}
