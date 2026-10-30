#include "dvd_file.h"
#include "archive.h"
#include "disk_error.h"

extern void OSReport(const char *, ...);
extern s32 dOpen;
extern s32 rCbkR;
extern void rCbk(s32 result, DVDFileInfo *file);
extern void *malloc2(u32 size);
extern void free(void *memory);
extern void *sReadFileLen(char *name, s32 *size);
extern s32 usbForce;
extern void *sReadFileUSB(char *name);
extern void *sReadFileLenUSB(char *name, s32 *size);

#pragma dont_inline on
s32 chkVS3(void);

s32 sReadPre(void)
{
    if (dOpen == 0 && DVDCheckDisk())
        return 0;
    if (dOpen != 0) {
        DVDCheckDisk();
        OSReport("PreOpen:");
        if (DVDCheckDisk()) {
            OSReport("Cl:");
            if (!chkVS3()) {
                OSReport("V3n");
                while (DVDCheckDisk())
                    diskError3(DISK_ERROR_NO_DISK);
            } else {
                dOpen = 0;
                OSReport("V3a");
                return 0;
            }
        } else
            OSReport("Op:");
    }
    diskError(DISK_ERROR_NONE);
    OSReport("PreRet:");
    dOpen = 0;
    return 1;
}

s32 fileLen(char *name)
{
    DVDFileInfo file;
    s32 length;
    if (!DVDOpen(name, &file))
        return 0;
    length = file.length;
    DVDClose(&file);
    return length;
}

s32 fReadO2(DVDFileInfo *file, void *memory, s32 length, s32 offset);



static s32 fReadO(DVDFileInfo *file, void *memory, s32 length)
{
    return fReadO2(file, memory, length, 0);
}

HSD_Archive *sArchiveLoad(char *name, void **data);

HSD_Archive *sLoad(char *name, void **data)
{
    sReadPre();
    return sArchiveLoad(name, data);
}

static void *sReadFile(char *name)
{
    sReadPre();
    return sReadFileLen(name, 0);
}

void *sReadFileU(char *name, s32 usb)
{
    if (usbForce != 0)
        usb = 1;
    if (usb != 0)
        return sReadFileUSB(name);
    return sReadFile(name);
}

void *sReadFileLenU(char *name, s32 usb, s32 *size)
{
    if (usbForce != 0)
        usb = 1;
    if (usb != 0)
        return sReadFileLenUSB(name, size);
    return sReadFileLen(name, size);
}
