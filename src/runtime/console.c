/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common_Embedded/uart_console_io.c.
 */
#include "runtime_stdio.h"

unsigned long WriteUARTN(void *buf, unsigned long len);
unsigned long ReadUARTN(void *bytes, unsigned long length);
unsigned long InitializeUART(unsigned long baudRate);

static inline int __init_uart_console(void)
{
    static int initialized = 0;
    int ret = 0;

    if (initialized == 0) {
        ret = InitializeUART(57600);

        if (ret == 0) {
            initialized = 1;
        }
    }

    return ret;
}

int __read_console(__file_handle handle, unsigned char *data, size_t *size,
                   __idle_proc idle)
{
    size_t len;
    int err;

    if (__init_uart_console() != 0) {
        return 1;
    }

    len = *size;
    err = 0;
    for (*size = 0; *size <= len && err == 0; ++data) {
        err = ReadUARTN(data, 1);
        ++*size;
        if (*data == '\r')
            break;
    }

    return (unsigned char)(err == 0 ? 0 : 1);
}

int __write_console(__file_handle handle, unsigned char *data, size_t *size,
                    __idle_proc idle)
{
    if (__init_uart_console() != 0) {
        return 1;
    }

    if ((long)WriteUARTN(data, *size) != 0) {
        *size = 0;
        return 1;
    }

    return 0;
}

int __close_console(__file_handle handle)
{
    return 0;
}
