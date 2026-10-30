/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common/wchar_io.c.
 */
#include "runtime_stdio.h"

int fwide(FILE *file, int mode)
{
    if (file->file_mode.file_kind == __closed_file) {
        return 0;
    }

    switch (file->file_mode.file_orientation) {
    case UNORIENTED:
        if (mode > 0) {
            file->file_mode.file_orientation = WIDE_ORIENTED;
        } else if (mode < 0) {
            file->file_mode.file_orientation = CHAR_ORIENTED;
        }
        return mode;
    case WIDE_ORIENTED:
        return 1;
    case CHAR_ORIENTED:
        return -1;
    }
}
