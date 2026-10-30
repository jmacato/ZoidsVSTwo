extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"
#include "gx_tev.h"
#include "joint.h"
#include "os_font.h"
#include "cobj.h"
#include "video.h"
#include "vi.h"
#include "gx_state.h"
#include "gx_geometry.h"

extern int sprintf(char *s, const char *fmt, ...);
extern void *LoadArc2(char *name);
extern void sMakeMatrix(HSD_JObj *joint);
extern s32 getAscNum1(s8 character);
extern void sPutFontRectPrologue(GXColor color, GXColor shadow, s32 alpha, u32 flags);
extern void sPutFontRectEpilogue(void);
extern void DrawFontChar00(char *string, HSD_JObj *joint, f32 scale);
extern void *sPad1;
extern s8 *asc00;
extern s8 *asc01;
extern s32 sPutFontCamPrologue(void);
extern void sPutStringXYC2(char *text, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, GXColor color,
                           GXColor shadow, s32 alpha, s32 flags, s32 align);
extern void HSD_StateInvalidate(s32 state);
extern void sPadAll(void);
extern void resetSkipOp(void);

#pragma force_active on
static OSFontHeader *FontData;
static void *LastSheet;
static s16 FontSize;
static s16 FontSpace;
static s32 camFont;
static s32 sStringNow;
static void *ascFile[5];
static f32 ascH[5][400];
static s32 sString[8000];
#pragma force_active off
#pragma force_active on
static u32 camFont_pos_wobjdesc[5] = {
    0x00000000, 0x41A00000, 0x41A00000, 0x41A00000, 0x00000000, 
};
static u32 camFont_interest_wobjdesc[5] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
u32 cameraFont[14] = {
    0x00000000, 0x00000001, 0x00000280, 0x000001E0, 0x00000280, 0x000001E0, 0x8011F748, 0x8011F75C, 
    0x00000000, 0x00000000, 0x3F8CCCCD, 0x465AC000, 0x42700000, 0x3FAA3D71, 
};
static const u32 lit_371[79] = {
    0x824F8250, 0x82518252, 0x82538254, 0x82558256, 0x82578258, 0x82608261, 0x82628263, 0x82648265, 
    0x82668267, 0x82688269, 0x826A826B, 0x826C826D, 0x826E826F, 0x82708271, 0x82728273, 0x82748275, 
    0x82768277, 0x82788279, 0x815E817C, 0x8169816A, 0x81958149, 0x8148828D, 0x83BF83C0, 0x8140817B, 
    0x81978197, 0x83418343, 0x83458347, 0x83498394, 0x834A834C, 0x834E8350, 0x8352834B, 0x834D834F, 
    0x83518353, 0x83548356, 0x8358835A, 0x835C8355, 0x83578359, 0x835B835D, 0x835E8360, 0x83638365, 
    0x8367835F, 0x83618364, 0x83668368, 0x8369836A, 0x836B836C, 0x836D836E, 0x83718374, 0x8377837A, 
    0x836F8372, 0x83758378, 0x837B837D, 0x837E8380, 0x83818382, 0x83708373, 0x83768379, 0x837C8384, 
    0x83868388, 0x8389838A, 0x838B838C, 0x838D838F, 0x83928393, 0x815B8340, 0x83428344, 0x83468348, 
    0x83628383, 0x83858387, 0x88EA94CA, 0x95BA8BA4, 0x98618D91, 0x92E98D91, 0x88EB93F3, 0x8EAE8B8C, 
    0x8C5E8CC0, 0x92E897CA, 0x8E598145, 0x8755828B, 0x91E5906C, 0x8E718B9F, 0x00000000, 
};
static const char lit_372[133] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ/-()&!?m@* +@@\xb1\xb2\xb3\xb4\xb5@\xb6\xb7\xb8\xb9\xba@@@@@\xbb\xbc\xbd\xbe\xbf@@@@@\xc0\xc1\xc2\xc3\xc4@@@@@\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce@@@@@\xcf\xd0\xd1\xd2\xd3@@@@@\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xa6\xdd\xb0\xa7\xa8\xa9\xaa\xab\xaf\xac\xad\xae";
#pragma force_active off
s32 getAscNum(char *text)
{
    s8 first = text[0], second = text[1];
    u8 lead = first & 0xE0;
    s8 *characters;
    s32 index;

    if (lead != 0xE0 && lead != 0x80)
        return getAscNum1(first);
    characters = asc00;
    if (first == 0)
        return -1;
    if (second == 0)
        return -1;
    index = 0;
    while (*characters) {
        if (first == characters[0] && second == characters[1])
            return index;
        characters += 2;
        index++;
    }
    return -1;
}
void ascPrologue(void)
{
    char name[0x100];
    s32 i;
    s32 j;
    HSD_JObj *jobj;

    for (i = 0; i < 5; i++) {
        sprintf(name, "ascii/ascii%02d.dat", i);
        ascFile[i] = LoadArc2(name);
        if (ascFile[i] != 0) {
            jobj = *(HSD_JObj **)((u8 *)ascFile[i] + 0x3c);
            for (j = 0; j < 400; j++) {
                ascH[i][j] = 0.0f;
            }
            if (jobj != (HSD_JObj *)0) {
                sMakeMatrix(jobj);
                jobj = jobj ? jobj->child : 0;
                for (j = 0; jobj != 0; j++) {
                    jobj ? (void)0 : __assert("jobj.h", 1009, "jobj");
                    ascH[i][j] = -jobj->translation.x;
                    jobj = jobj ? jobj->next : 0;
                }
            }
        }
    }
}
#undef lit_417
#define lit_417 "ascii/ascii%02d.dat"
#undef lit_419
#define lit_419 "jobj.h"
#undef lit_420
#define lit_420 "jobj"
#pragma force_active on
static const u32 lit_681[3] = {
    0x00000000, 0x00000000, 0x00000000, 
};
static const u32 lit_682[3] = {
    0x00000000, 0x00000000, 0xC5BB8000, 
};
#pragma force_active off
#pragma dont_inline off
static inline void GXPosition3f32(f32 x, f32 y, f32 z)
{
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
    GXWGFifo.f32 = z;
}

static inline void GXTexCoord2s16(s16 s, s16 t)
{
    GXWGFifo.s16 = s;
    GXWGFifo.s16 = t;
}

void DrawFontCharXY(f32 x, f32 y, f32 z, s32 s, s32 t, s32 width, f32 sx, f32 sy)
{
    f32 x1 = x;
    f32 x2;
    f32 y1 = y;
    f32 y2;
    s16 s1;
    s16 t1;
    s16 s2;
    s16 t2;

    (void)sx;
    (void)sy;
    s1 = s;
    t1 = t;
    s2 = s + width;
    x2 = x1 + width * (FontSize / 16.0 * sx) / FontData->cellWidth;
    y2 = y - FontSize * (FontData->cellHeight / 16.0 * sy) / FontData->cellWidth;
    t2 = t + FontData->cellHeight;

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x1, y1, z);
    GXTexCoord2s16(s1, t1);
    GXPosition3f32(x2, y1, z);
    GXTexCoord2s16(s2, t1);
    GXPosition3f32(x2, y2, z);
    GXTexCoord2s16(s2, t2);
    GXPosition3f32(x1, y2, z);
    GXTexCoord2s16(s1, t2);
}
#pragma dont_inline on
void sPutFontRect(char *string, HSD_JObj *top_left, HSD_JObj *bottom_right, GXColor color, GXColor shadow,
                  s32 alpha, u32 flags)
{
    s32 width;
    void *image;
    s32 s;
    s32 t;
    Vec3 eye;
    char *p;
    s32 widest;
    s32 line_width;
    s32 height;
    f32 scale_x;
    f32 scale_y;
    s32 k;

    if (top_left != 0 && bottom_right != 0 && string != 0 && *string != 0) {
        widest = 0;
        height = 0;
        line_width = 0;
        LastSheet = 0;
        p = string;
        while (*p != 0) {
            if (*p == '\n') {
                p++;
            } else if (*p == '\r') {
                line_width = 0;
                p++;
                height += 24;
            } else {
                p = OSGetFontTexture(p, &image, &s, &t, &width);
                line_width += width;
                if (widest < line_width)
                    widest = line_width;
            }
        }
        height += 24;
        if (widest != 0) {
            scale_x = (bottom_right->matrix[0][3] - top_left->matrix[0][3]) / (f32)widest;
            scale_y = (top_left->matrix[1][3] - bottom_right->matrix[1][3]) / (f32)height;
            if (scale_x > scale_y)
                scale_x = scale_y;
            sPutFontRectPrologue(color, shadow, alpha, flags);
            HSD_CObjGetEyePosition(HSD_CObjGetCurrent(), &eye);
            DrawFontChar00(string, top_left, scale_x);
            sPutFontRectEpilogue();
            k = 0;
            (void)k;
        }
    }
}
#pragma force_active on
static const f32 lit_544[1] = { -1.714704e38f };
static const f32 lit_548[1] = { 1.0f };
static const f32 lit_551[1] = { -1.7146928e38f };
static const f32 lit_568[1] = { 15.0f };
static const f32 lit_569[1] = { 0.0625f };
static const f32 lit_631[1] = { 0.5f };
static const f32 lit_704[1] = { 1000.0f };
static const f32 lit_718[1] = { -1.7146928e38f };
static const f32 lit_728[1] = { 24.0f };
#pragma force_active off
void EndeFin(void)
{
    s32 i;

    for (i = 0; i < 600; i++) {
        HSD_StartRender(HSD_RP_SCREEN);
        if (sPutFontCamPrologue() != 0) {
            sPutStringXYC2("B A T T L E  A L L  O V E R .", 0.0f, 0.0f, -200.0f, 0.6f, 0.7f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF },
                           (GXColor){ 0x00, 0x00, 0x32, 0x00 }, 0, 2, 3);
            sPutStringXYC2("Thank you for your playing!", 0.0f, -50.0f, -250.0f, 0.6f, 0.7f,
                           (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF }, (GXColor){ 0x00, 0x00, 0x32, 0x00 }, 0, 2, 3);
            HSD_StateInvalidate(-1);
            HSD_CObjEndCurrent();
        }
        HSD_EndRender();
        HSD_VICopyXFB(HSD_RP_SCREEN);
        HSD_VISetXFBDrawDone();
        VIWaitForRetrace();
        sPadAll();
        if (*(s8 *)((u8 *)sPad1 + 0x63) != 0) {
            break;
        }
    }
    resetSkipOp();
}
#undef lit_790
#define lit_790 "B A T T L E  A L L  O V E R ."
#undef lit_794
#define lit_794 "Thank you for your playing!"
#pragma force_active on
static s8 *asc00 = (s8 *)lit_371;
static s8 *asc01 = (s8 *)lit_372;
#pragma force_active off
}
