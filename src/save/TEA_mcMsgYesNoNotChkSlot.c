#pragma readonly_strings on
#include "types.h"

typedef struct PadStateVS3 {
    u8 unk[0x5d];
    s8 pressed_a;
} PadStateVS3;
#include "menu_message.h"

extern s32 menudispyesnopoint;
extern s32 menudispyesnoflag;
extern PadStateVS3 *sPad1, *sPad2;
extern void clrYesNoMsg(void);
extern char *GetMenuMsgAdr(s16 message);
extern void menuYesNoSetStr(const char *first, const char *second);
extern s32 menuYesNo(s32 selected, const char *first, const char *second);
extern void menu_sDispStartYesNo(void);
extern void OSReport(const char *format, ...);
extern void sPadAll(void);
extern void SeRet(void);
extern s32 TEA_mcChkSlot(s32 channel);
void TEA_mcMsgNoChkSlot(s32 slot, s16 message);

s32 TEA_mcMsgYesNoNotChkSlot(s32 slot, s16 message)
{
    menudispyesnopoint = 1;
    clrYesNoMsg();
    menudispyesnoflag = 1;
    menuYesNoSetStr("", GetMenuMsgAdr(message));
    OSReport("Memcard MsgWaitPutNotSlotChk:SLOT=%d MSGNO=%d\n", slot, message);
    do {
        sPadAll();
        menu_sDispStartYesNo();
        menudispyesnopoint = menuYesNo(menudispyesnopoint, "", GetMenuMsgAdr(message));
    } while (sPad1->pressed_a == 0 && sPad2->pressed_a == 0);
    SeRet();
    clrYesNoMsg();
    menuYesNoSetStr("", "");
    if (menudispyesnopoint == 1) {
        return 1;
    }
    return 0;
}
