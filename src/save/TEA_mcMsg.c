#pragma readonly_strings on
#include "types.h"
#include "menu_message.h"

typedef struct PadPressed {
    u8 pad_0[0x5d];
    s8 pressed_a;
} PadPressed;

extern s32 menudispyesnopoint;
extern s32 menudispyesnoflag;
extern PadPressed *sPad1, *sPad2;
extern void clrYesNoMsg(void);
extern char *GetMenuMsgAdr(s16 message);
extern void menuYesNoSetStr(const char *first, const char *second);
extern void menu_sDispStartYesNo(void);
extern void sPadAll(void);
extern void SeRet(void);

void TEA_mcMsg(s32 slot, s16 message)
{
    s32 spare = slot;
    clrYesNoMsg();
    menudispyesnopoint = 1;
    menudispyesnoflag = 1;
    menuYesNoSetStr(GetMenuMsgAdr(message), GetMenuMsgAdr(message));
    do {
        sPadAll();
        menu_sDispStartYesNo();
        menuYesNoSetStr(GetMenuMsgAdr(message), GetMenuMsgAdr(message));
    } while (sPad1->pressed_a == 0 && sPad2->pressed_a == 0);
    SeRet();
    clrYesNoMsg();
    menu_sDispStartYesNo();
}
