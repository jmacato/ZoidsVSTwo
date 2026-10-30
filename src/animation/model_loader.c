#include "model_archive.h"

extern s32 usbForce;
extern s32 LoadArc(ModelArchive *archive, char *name);
extern s32 LoadArcUSB(ModelArchive *archive, char *name);
extern ModelArchive *LoadArc2_(char *name, s32 usb);

#pragma dont_inline on
s32 LoadArcU(ModelArchive *archive, char *name, s32 usb)
{
    if (usbForce != 0)
        usb = 1;
    if (usb != 0)
        return LoadArcUSB(archive, name);
    return LoadArc(archive, name);
}

ModelArchive *LoadArc2(char *name)
{
    s32 usb = 0;
    if (usbForce != 0)
        usb = 1;
    return LoadArc2_(name, usb);
}

ModelArchive *LoadArc2USB(char *name)
{
    return LoadArc2_(name, 1);
}
