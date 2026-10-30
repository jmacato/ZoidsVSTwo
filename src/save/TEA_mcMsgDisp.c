#pragma readonly_strings on
#include "game_pad.h"
#include "menu_message.h"

extern s32 menudispyesnopoint;
extern s32 menudispyesnoflag;
extern void clrYesNoMsg(void);
extern char *GetMenuMsgAdr(s16 message);
extern s32 menuYesNo(s32 selected, const char *first, const char *second);
extern void menu_sDispStartYesNo(void);

void TEA_mcMsgDisp(s32 slot, s16 message)
{
    s32 spare = slot;
    menudispyesnopoint = 1;
    clrYesNoMsg();
    menudispyesnoflag = 1;
    menudispyesnopoint = menuYesNo(menudispyesnopoint, GetMenuMsgAdr(message), GetMenuMsgAdr(message));
    menu_sDispStartYesNo();
}
