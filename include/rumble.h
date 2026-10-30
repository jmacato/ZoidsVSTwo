/* Layouts from
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/rumble.h. */
#ifndef ZOIDS_RUMBLE_H
#define ZOIDS_RUMBLE_H
#include "types.h"
typedef struct HSD_PadRumbleListData HSD_PadRumbleListData;
typedef struct HSD_RumbleData {
    u8 last_status;
    u8 status;
    u8 direct_status;
    u16 nb_list;
    HSD_PadRumbleListData *listdatap;
} HSD_RumbleData;

typedef struct RumbleCommand {
    u16 op : 3;
    u16 frame : 13;
} RumbleCommand;

typedef union HSD_Rumble {
    u16 def;
    RumbleCommand command;
} HSD_Rumble;

typedef struct RumbleInfo {
    u16 max_list;
    u8 reserved_02;
    HSD_PadRumbleListData *listdatap;
} RumbleInfo;

struct HSD_PadRumbleListData {
    HSD_PadRumbleListData *next;
    u32 id;
    u8 pause;
    u8 pri;
    u8 status;
    u16 loop_count;
    u16 wait;
    s32 frame;
    HSD_Rumble *stack;
    HSD_Rumble *listp;
    HSD_Rumble *headp;
};

extern void HSD_PadRumbleOn(u8 channel);
extern void HSD_PadRumbleOffN(u8 channel);
extern void HSD_PadRumbleInterpret(void);
extern void HSD_PadRumbleInit(u16 count, void *records);
#endif
