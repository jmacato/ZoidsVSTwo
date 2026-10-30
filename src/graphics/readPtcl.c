#pragma readonly_strings on
#include "types.h"

extern u16 *pTcmdList;
extern void *pTtexGroup;
extern void *pTref;
extern char *strcpy(char *destination, const char *source);
extern char *strcat(char *destination, const char *source);
extern s32 fileRead(char *path, void *destination);
extern void DCFlushRange(void *address, u32 size);
extern void OSReport(const char *format, ...);
extern void psInitDataBank(s32 bank, u16 *commands, void *textures, void *references, s32 flags);

s32 readPtcl(char *name)
{
    char path[80];
    s32 size;
    u16 *commands;

    pTcmdList = 0;
    pTtexGroup = 0;
    pTref = 0;
    strcpy(path, name);
    strcat(path, ".ptl");
    if (fileRead(path, &pTcmdList) < 0) {
        return -1;
    }
    strcpy(path, name);
    strcat(path, ".txg");
    if ((size = fileRead(path, &pTtexGroup)) < 0) {
        return -1;
    }
    DCFlushRange(pTtexGroup, (size + 31) & ~31);
    strcpy(path, name);
    strcat(path, ".ref");
    if (fileRead(path, &pTref) < 0) {
        return -1;
    }
    commands = pTcmdList;
    if ((s32)commands[0] != 67) {
        OSReport("unmatch versions between system(%d) and ptcl-file(%d)", 67, commands[0]);
    }
    psInitDataBank(commands[1], pTcmdList, pTtexGroup, pTref, 0);
    return 0;
}
