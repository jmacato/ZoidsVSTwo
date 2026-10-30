#ifndef ZOIDS_GAME_TEXT_H
#define ZOIDS_GAME_TEXT_H

#include "model_archive.h"

#define ASC_FONT_COUNT 5
#define ASC_GLYPH_MAX 400

/* one model archive per ascii/ascii%02d.dat */
extern ModelArchive *ascFile[ASC_FONT_COUNT];
/* advance width of each glyph joint, set by ascPrologue */
extern f32 ascH[ASC_FONT_COUNT][ASC_GLYPH_MAX];
/* glyph order of the ascii models: two-byte Shift-JIS characters (asc00) and one-byte characters (asc01) */
extern s8 *asc00;
extern s8 *asc01;

extern s32 isKanji(u8 character);
extern s32 getAscNum1(s8 character);
extern s32 getAscNum(char *text);
extern f32 getAscH_sp(s32 font, char *text, f32 spacing);
extern f32 putAscLR_sp(Vec3 *position, s32 font, char *text, s32 right_align, f32 spacing);
extern f32 putAscMtxLR(f32 matrix[3][4], s32 font, char *text, s32 right_align);
extern f32 putAscMtxLR_sp(f32 matrix[3][4], s32 font, char *text, s32 right_align, f32 spacing);
extern f32 putAscJobj(HSD_JObj *joint, s32 font, char *text);
extern f32 putAscJobjLR(HSD_JObj *joint, s32 font, char *text, s32 right_align);
extern f32 putAscJobjLR_sp(HSD_JObj *joint, s32 font, char *text, s32 right_align, f32 spacing);

#endif
