#ifndef ZOIDS_DISK_ERROR_H
#define ZOIDS_DISK_ERROR_H

#include "types.h"

/* index into diskErrMsg drawn by diskError3; 0 draws nothing */
enum DiskErrorMessage {
    DISK_ERROR_NONE = 0,
    DISK_ERROR_COVER_OPEN = 1,
    DISK_ERROR_NO_DISK = 2,
    DISK_ERROR_WRONG_DISK = 3,
    DISK_ERROR_RETRY = 4,
    DISK_ERROR_FATAL = 5,
    DISK_ERROR_OTHER = 6
};

extern void diskError(s32 mode);
extern void diskError2(s32 error);
extern void diskError3(s32 message);

#endif
