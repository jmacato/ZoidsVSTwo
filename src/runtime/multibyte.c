/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common/mbstring.c.
 */
#include "runtime_string.h"
typedef unsigned short wchar_t;

size_t wcstombs(char *s, const wchar_t *pwcs, size_t n)
{
    wchar_t next;
    size_t chars_written;
    int i;

    chars_written = 0;
    for (i = 0; i < n; ++i) {
        next = *pwcs++;
        *s++ = (char)next;
        if ((char)next == '\0')
            break;
        ++chars_written;
    }
    return chars_written;
}
