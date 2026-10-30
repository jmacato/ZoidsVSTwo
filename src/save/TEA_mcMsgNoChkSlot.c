#pragma readonly_strings on
#include "types.h"

typedef struct PadStateVS3 {
    u8 unk[0x5d];
    s8 pressed_a;
} PadStateVS3;

extern s32 menudispyesnopoint;
extern s32 menudispyesnoflag;
extern PadStateVS3 *sPad1, *sPad2;
extern void clrYesNoMsg(void);
extern char *GetMenuMsgAdr(s16 message);
extern void menuYesNoSetStr(const char *first, const char *second);
extern s32 menuYesNo(s32 selected, const char *first, const char *second);
extern void menu_sDispStartYesNo(void);
extern s32 printf(const char *format, ...);
extern void sPadAll(void);
extern void SeRet(void);
extern s32 TEA_mcChkSlot(s32 channel);

void TEA_mcMsgNoChkSlot(s32 slot, s16 message)
{
    s32 frame_slot = slot;

    menudispyesnopoint = 1;
    clrYesNoMsg();
    menudispyesnoflag = 1;
    menuYesNoSetStr(GetMenuMsgAdr(message), GetMenuMsgAdr(message));
    do {
        sPadAll();
        menu_sDispStartYesNo();
        menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(message), GetMenuMsgAdr(message));
        if (TEA_mcChkSlot(slot) == 0) {
            printf("\203\201\203\202\203JMSGOUT2 \203X\203\215\203b\203g\226\263\202\265 %d[%d]", slot, message);
            break;
        }
    } while (sPad1->pressed_a == 0 && sPad2->pressed_a == 0);
    SeRet();
    clrYesNoMsg();
    menu_sDispStartYesNo();
}
