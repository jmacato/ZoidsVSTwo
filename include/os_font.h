/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/os/OSFont.h. */
#ifndef ZOIDS_OS_FONT_H
#define ZOIDS_OS_FONT_H
#include "types.h"
#define OS_FONT_ENCODE_ANSI 0u
#define OS_FONT_ENCODE_SJIS 1u
#define OS_FONT_ROM_SIZE_ANSI 0x03000
#define OS_FONT_ROM_SIZE_SJIS 0x4D000
#define OS_FONT_SIZE_ANSI (288 + 131072)
#define OS_FONT_SIZE_SJIS (3840 + 1179648)
typedef struct OSFontHeader {
    u16 fontType, firstChar, lastChar, invalChar;
    u16 ascent, descent, width, leading;
    u16 cellWidth, cellHeight;
    u32 sheetSize;
    u16 sheetFormat, sheetColumn, sheetRow, sheetWidth;
    u16 sheetHeight, widthTable;
    u32 sheetImage, sheetFullSize;
    u8 c0, c1, c2, c3;
} OSFontHeader;
extern u16 OSGetFontEncode(void);
extern u32 OSLoadFont(OSFontHeader *font, void *temporary);
extern s32 OSInitFont(OSFontHeader *font);
extern char *OSGetFontTexture(char *string, void **image, s32 *x, s32 *y, s32 *width);
#endif
