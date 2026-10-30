#ifndef ZOIDS_CARD_STATUS_H
#define ZOIDS_CARD_STATUS_H
#include "card.h"
#define CARD_ICON_MAX 8
#define CARD_STAT_ICON_NONE 0
#define CARD_STAT_ICON_C8 1
#define CARD_STAT_ICON_RGB5A3 2
#define CARD_STAT_ICON_MASK 3
#define CARD_STAT_BANNER_NONE 0
#define CARD_STAT_BANNER_C8 1
#define CARD_STAT_BANNER_RGB5A3 2
#define CARD_STAT_BANNER_MASK 3
#define CARD_STAT_ANIM_LOOP 0x00
#define CARD_STAT_ANIM_BOUNCE 0x04
#define CARD_STAT_ANIM_MASK 0x04
#define CARD_STAT_SPEED_END 0
#define CARD_STAT_SPEED_FAST 1
#define CARD_STAT_SPEED_MIDDLE 2
#define CARD_STAT_SPEED_SLOW 3
#define CARD_STAT_SPEED_MASK 3
typedef struct CARDStat {
    /*0x00*/ char fileName[CARD_FILENAME_MAX];
    /*0x20*/ u32 length;
    /*0x24*/ u32 time;
    /*0x28*/ u8 gameName[4];
    /*0x2C*/ u8 company[2];
    /*0x2E*/ u8 bannerFormat;
    /*0x30*/ u32 iconAddr;
    /*0x34*/ u16 iconFormat;
    /*0x36*/ u16 iconSpeed;
    /*0x38*/ u32 commentAddr;
    /*0x3C*/ u32 offsetBanner;
    /*0x40*/ u32 offsetBannerTlut;
    /*0x44*/ u32 offsetIcon[CARD_ICON_MAX];
    /*0x64*/ u32 offsetIconTlut;
    /*0x68*/ u32 offsetData;
} CARDStat;

extern s32 CARDGetStatus(s32 chan, s32 file_number, CARDStat *status);
extern s32 CARDSetStatusAsync(s32 chan, s32 file_number, CARDStat *status,
                              CARDCallback callback);
extern s32 CARDSetStatus(s32 chan, s32 file_number, CARDStat *status);
#endif
