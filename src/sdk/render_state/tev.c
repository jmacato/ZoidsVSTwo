/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/tev.c.
 */
#include "state.h"
#define NULL 0
#define GX_SRC_REG 0
#define GX_TEV_SWAP0 0
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if ((condition) == 0)                                                          \
            __assert("tev.c", line, #condition);                                       \
    } while (0)
typedef s32 bool;
typedef s32 GXTevMode;
static struct {
    GXColorS10 a;
    s32 c;
} TevReg[4] = {0};
static HSD_Chan prev_ch[4] = {
    {NULL, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
    {NULL, 1, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
    {NULL, 2, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
    {NULL, 3, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL}};
static HSD_ObjAllocData rd_alloc_data __attribute__((aligned(8)));
static HSD_ObjAllocData reg_alloc_data;
static HSD_ObjAllocData ch_alloc_data;
static s32 current_tev;
static s32 num_tex_gens;
static s32 prev_num_chans;
static s32 prev_amb_invalid[2];
static s32 prev_mat_invalid[2];
extern void GXSetChanAmbColor(GXChannelID, GXColor);
extern void GXSetChanMatColor(GXChannelID, GXColor);
extern void GXSetChanCtrl(GXChannelID, u8, s32, s32, u32, s32, s32);
extern void GXSetNumChans(u8);
extern void GXSetNumTexGens(u8);
extern void GXSetNumTevStages(u8);
extern void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
extern void GXSetTevOp(GXTevStageID, s32);
extern void GXSetTevSwapMode(GXTevStageID, GXTevSwapSel, GXTevSwapSel);
extern void GXSetTevKColorSel(GXTevStageID, GXTevKColorSel);
extern void GXSetTevKAlphaSel(GXTevStageID, GXTevKAlphaSel);
extern void GXSetTevColorS10(GXTevRegID, GXColorS10);
void HSD_RenderInitAllocData(void)
{
    HSD_ObjAllocInit(&rd_alloc_data, 28, 4);
    HSD_ObjAllocInit(&reg_alloc_data, 20, 4);
    HSD_ObjAllocInit(&ch_alloc_data, sizeof(HSD_Chan), 4);
}







static inline bool CompareRGB(GXColor *c0, GXColor *c1)
{
    u32 *d0 = (u32 *)c0;
    u32 *d1 = (u32 *)c1;
    return ((*d0 ^ *d1) & 0xFFFFFF00) != 0;
}

static inline bool CompareRGBA(GXColor *c0, GXColor *c1)
{
    u32 *d0 = (u32 *)c0;
    u32 *d1 = (u32 *)c1;
    return *d0 != *d1;
}

static inline void CopyRGB(GXColor *dst, GXColor *src)
{
    u32 *d = (u32 *)dst;
    u32 *s = (u32 *)src;
    *d = (*d & 0xff) | (*s & 0xffffff00);
}

void HSD_SetupChannel(HSD_Chan *ch)
{
    int idx;
    GXChannelID chan;
    int no;

    if (ch == NULL || ch->chan == GX_COLOR_NULL) {
        return;
    }

    chan = ch->chan;
    idx = chan & 3;
    no = chan & 1;
    if (ch->enable != GX_DISABLE && ch->amb_src == GX_SRC_REG) {
        if (prev_amb_invalid[no] != 0) {
            prev_amb_invalid[no] = 0;
            GXSetChanAmbColor(no + 4, ch->amb_color);
            prev_ch[no].amb_color = ch->amb_color;
        } else if (chan == GX_COLOR0A0 || chan == GX_COLOR1A1) {
            if (CompareRGBA(&ch->amb_color, &prev_ch[no].amb_color)) {
                prev_ch[no].amb_color = ch->amb_color;
                goto set_amb;
            }
        } else if (chan == GX_COLOR0 || chan == GX_COLOR1) {
            if (CompareRGB(&ch->amb_color, &prev_ch[no].amb_color)) {
                CopyRGB(&prev_ch[no].amb_color, &ch->amb_color);
                goto set_amb;
            }
        } else if (ch->amb_color.a != prev_ch[no].amb_color.a) {
            prev_ch[no].amb_color.a = ch->amb_color.a;
        set_amb:
            GXSetChanAmbColor(chan, ch->amb_color);
        }
    }

    if (ch->mat_src == GX_SRC_REG) {
        if (prev_mat_invalid[no] != 0) {
            prev_mat_invalid[no] = 0;
            GXSetChanMatColor(no + 4, ch->mat_color);
            prev_ch[no].mat_color = ch->mat_color;
        } else if (chan == GX_COLOR0A0 || chan == GX_COLOR1A1) {
            if (CompareRGBA(&ch->mat_color, &prev_ch[no].mat_color)) {
                prev_ch[no].mat_color = ch->mat_color;
                goto set_mat;
            }
        } else if (chan == GX_COLOR0 || chan == GX_COLOR1) {
            if (CompareRGB(&ch->mat_color, &prev_ch[no].mat_color)) {
                CopyRGB(&prev_ch[no].mat_color, &ch->mat_color);
                goto set_mat;
            }
        } else if (ch->mat_color.a != prev_ch[no].mat_color.a) {
            prev_ch[no].mat_color.a = ch->mat_color.a;
        set_mat:
            GXSetChanMatColor(chan, ch->mat_color);
        }
    }

    if ((ch->enable != prev_ch[idx].enable) || (ch->amb_src != prev_ch[idx].amb_src) ||
        (ch->mat_src != prev_ch[idx].mat_src) ||
        (ch->light_mask != prev_ch[idx].light_mask) ||
        (ch->diff_fn != prev_ch[idx].diff_fn) ||
        (ch->attn_fn != prev_ch[idx].attn_fn)) {
        GXSetChanCtrl(chan, ch->enable, ch->amb_src, ch->mat_src, ch->light_mask,
                      ch->diff_fn, ch->attn_fn);
        prev_ch[idx].enable = ch->enable;
        prev_ch[idx].amb_src = ch->amb_src;
        prev_ch[idx].mat_src = ch->mat_src;
        prev_ch[idx].light_mask = ch->light_mask;
        prev_ch[idx].diff_fn = ch->diff_fn;
        prev_ch[idx].attn_fn = ch->attn_fn;
        if (chan == GX_COLOR0A0 || chan == GX_COLOR1A1) {
            prev_ch[idx + 2].enable = ch->enable;
            prev_ch[idx + 2].amb_src = ch->amb_src;
            prev_ch[idx + 2].mat_src = ch->mat_src;
            prev_ch[idx + 2].light_mask = ch->light_mask;
            prev_ch[idx + 2].diff_fn = ch->diff_fn;
            prev_ch[idx + 2].attn_fn = ch->attn_fn;
        }
    }
}

void HSD_DisableChannelLighting(GXChannelID chan)
{
    int idx = chan & 3;
    switch (chan) {
    case GX_COLOR0A0:
    case GX_COLOR1A1:
        if (prev_ch[idx].enable != 0 || prev_ch[idx + 2].enable != 0) {
            prev_ch[idx].enable = prev_ch[idx + 2].enable = 0;
            prev_ch[idx].light_mask = prev_ch[idx + 2].light_mask = 0;
            {
                s32 ambient = prev_ch[idx].amb_src;
                s32 material = prev_ch[idx].mat_src;
                s32 diffuse = prev_ch[idx].diff_fn;
                s32 attenuation = prev_ch[idx].attn_fn;
                prev_ch[idx + 2].amb_src = ambient;
                prev_ch[idx + 2].mat_src = material;
                prev_ch[idx + 2].diff_fn = diffuse;
                prev_ch[idx + 2].attn_fn = attenuation;
                GXSetChanCtrl(chan, 0, ambient, material, 0, diffuse, attenuation);
            }
        }
        break;
    default:
        if (prev_ch[idx].enable != 0) {
            prev_ch[idx].enable = 0;
            prev_ch[idx].light_mask = 0;
            GXSetChanCtrl(chan, 0, prev_ch[idx].amb_src, prev_ch[idx].mat_src, 0,
                          prev_ch[idx].diff_fn, prev_ch[idx].attn_fn);
        }
        break;
    }
}

void HSD_StateSetNumChans(int num)
{
    if (prev_num_chans != num) {
        GXSetNumChans(num);
        prev_num_chans = num;
    }
}

void HSD_SetupChannelAll(HSD_Chan *channel)
{
    int num = 0;
    while (channel != NULL) {
        int channel_count = HSD_Channel2Num(channel->chan);
        if (channel_count > num) {
            num = channel_count;
        }
        HSD_SetupChannel(channel);
        channel = channel->next;
    }
    HSD_StateSetNumChans((u8)num);
}

void HSD_StateRegisterTexGen(int coord)
{
    int coordinate_count = HSD_TexCoordID2Num(coord);
    if (coordinate_count > num_tex_gens) {
        num_tex_gens = coordinate_count;
    }
}

void HSD_StateSetNumTexGens(void)
{
    GXSetNumTexGens(num_tex_gens);
    num_tex_gens = 0;
}

void HSD_StateInitTev(void)
{
    current_tev = 0;
}

int HSD_StateGetNumTevStages(void)
{
    return current_tev;
}

GXTevStageID HSD_StateAssignTev(void)
{
    return HSD_Index2TevStage(current_tev++);
}

void HSD_StateSetNumTevStages(void)
{
    GXSetNumTevStages(current_tev);
    current_tev = 0;
}

#pragma push
#pragma dont_inline on
void HSD_SetupTevStage(HSD_TevDesc *desc)
{
    u8 _[8];
    GXSetTevOrder(desc->stage, desc->coord, desc->map, desc->color);
    if (desc->flags == 0) {
        GXSetTevOp(desc->stage, (GXTevMode)desc->u.tevconf.clr_op);
        GXSetTevSwapMode(desc->stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        return;
    }
    GXSetTevColorOp(desc->stage, desc->u.tevconf.clr_op, desc->u.tevconf.clr_bias,
                    desc->u.tevconf.clr_scale, desc->u.tevconf.clr_clamp,
                    desc->u.tevconf.clr_out_reg);
    GXSetTevColorIn(desc->stage, desc->u.tevconf.clr_a, desc->u.tevconf.clr_b,
                    desc->u.tevconf.clr_c, desc->u.tevconf.clr_d);
    GXSetTevAlphaOp(desc->stage, desc->u.tevconf.alpha_op, desc->u.tevconf.alpha_bias,
                    desc->u.tevconf.alpha_scale, desc->u.tevconf.alpha_clamp,
                    desc->u.tevconf.alpha_out_reg);
    GXSetTevAlphaIn(desc->stage, desc->u.tevconf.alpha_a, desc->u.tevconf.alpha_b,
                    desc->u.tevconf.alpha_c, desc->u.tevconf.alpha_d);
    GXSetTevSwapMode(desc->stage, desc->u.tevconf.ras_swap, desc->u.tevconf.tex_swap);
    GXSetTevKColorSel(desc->stage, desc->u.tevconf.kcsel);
    GXSetTevKAlphaSel(desc->stage, desc->u.tevconf.kasel);
}
#pragma pop

static inline int setupTevStages(HSD_TevDesc *desc)
{
    int num = 0;

    while (desc != NULL) {
        int temp = HSD_TevStage2Num(desc->stage);
        if (temp > num) {
            num = temp;
        }
        HSD_SetupTevStage(desc);
        desc = desc->next;
    }
    return num;
}

void HSD_SetupTevStageAll(HSD_TevDesc *desc)
{
    current_tev = setupTevStages(desc);
    HSD_StateSetNumTevStages();
}

#pragma push
#pragma dont_inline on
int HSD_Channel2Num(int chan)
{
    switch (chan) {
    case GX_COLOR0:
        return 1;
    case GX_COLOR1:
        return 2;
    case GX_ALPHA0:
        return 1;
    case GX_ALPHA1:
        return 2;
    case GX_COLOR0A0:
        return 1;
    case GX_COLOR1A1:
        return 2;
    case GX_COLOR_NULL:
        return 0;
    default:
        HSD_ASSERT(721, 0);
        return 0;
    }
}
#pragma pop

#pragma push
#pragma dont_inline on
GXTevStageID HSD_Index2TevStage(int idx)
{
    switch (idx) {
    case 0:
        return GX_TEVSTAGE0;
    case 1:
        return GX_TEVSTAGE1;
    case 2:
        return GX_TEVSTAGE2;
    case 3:
        return GX_TEVSTAGE3;
    case 4:
        return GX_TEVSTAGE4;
    case 5:
        return GX_TEVSTAGE5;
    case 6:
        return GX_TEVSTAGE6;
    case 7:
        return GX_TEVSTAGE7;
    case 8:
        return GX_TEVSTAGE8;
    case 9:
        return GX_TEVSTAGE9;
    case 10:
        return GX_TEVSTAGE10;
    case 11:
        return GX_TEVSTAGE11;
    case 12:
        return GX_TEVSTAGE12;
    case 13:
        return GX_TEVSTAGE13;
    case 14:
        return GX_TEVSTAGE14;
    case 15:
        return GX_TEVSTAGE15;
    default:
        HSD_ASSERT(774, 0);
        return GX_TEVSTAGE15;
    }
}
#pragma pop

int HSD_TevStage2Index(int stage)
{
    switch (stage) {
    case GX_TEVSTAGE0:
        return 0;
    case GX_TEVSTAGE1:
        return 1;
    case GX_TEVSTAGE2:
        return 2;
    case GX_TEVSTAGE3:
        return 3;
    case GX_TEVSTAGE4:
        return 4;
    case GX_TEVSTAGE5:
        return 5;
    case GX_TEVSTAGE6:
        return 6;
    case GX_TEVSTAGE7:
        return 7;
    case GX_TEVSTAGE8:
        return 8;
    case GX_TEVSTAGE9:
        return 9;
    case GX_TEVSTAGE10:
        return 10;
    case GX_TEVSTAGE11:
        return 11;
    case GX_TEVSTAGE12:
        return 12;
    case GX_TEVSTAGE13:
        return 13;
    case GX_TEVSTAGE14:
        return 14;
    case GX_TEVSTAGE15:
        return 15;
    default:
        HSD_ASSERT(827, 0);
        return 0;
    }
}

#pragma push
#pragma dont_inline on
int HSD_TevStage2Num(int stage)
{
    switch (stage) {
    case GX_TEVSTAGE0:
        return 1;
    case GX_TEVSTAGE1:
        return 2;
    case GX_TEVSTAGE2:
        return 3;
    case GX_TEVSTAGE3:
        return 4;
    case GX_TEVSTAGE4:
        return 5;
    case GX_TEVSTAGE5:
        return 6;
    case GX_TEVSTAGE6:
        return 7;
    case GX_TEVSTAGE7:
        return 8;
    case GX_TEVSTAGE8:
        return 9;
    case GX_TEVSTAGE9:
        return 10;
    case GX_TEVSTAGE10:
        return 11;
    case GX_TEVSTAGE11:
        return 12;
    case GX_TEVSTAGE12:
        return 13;
    case GX_TEVSTAGE13:
        return 14;
    case GX_TEVSTAGE14:
        return 15;
    case GX_TEVSTAGE15:
        return 16;
    default:
        HSD_ASSERT(858, 0);
        return 0;
    }
}
#pragma pop

void HSD_SetTevRegAll(void)
{
    u32 i;
    for (i = 0; i < 4; i++) {
        if (TevReg[i].c != 0) {
            int reg;
            switch (i) {
            case 0:
                reg = GX_TEVREG0;
                break;
            case 1:
                reg = GX_TEVREG1;
                break;
            case 2:
                reg = GX_TEVREG2;
                break;
            case 3:
                reg = GX_TEVPREV;
                break;
            default:
                reg = GX_TEVREG0;
                break;
            }
            GXSetTevColorS10(reg, TevReg[i].a);
            TevReg[i].c = 0;
        }
    }
}

#pragma push
#pragma dont_inline on
int HSD_TexCoordID2Num(int id)
{
    switch (id) {
    case GX_TEXCOORD0:
        return 1;
    case GX_TEXCOORD1:
        return 2;
    case GX_TEXCOORD2:
        return 3;
    case GX_TEXCOORD3:
        return 4;
    case GX_TEXCOORD4:
        return 5;
    case GX_TEXCOORD5:
        return 6;
    case GX_TEXCOORD6:
        return 7;
    case GX_TEXCOORD7:
        return 8;
    case GX_TEXCOORD_NULL:
        return 0;
    default:
        HSD_ASSERT(1075, 0);
        return 0;
    }
}
#pragma pop

#pragma push
#pragma force_active on
void ChanUpdateFunc(HSD_Chan *chan, int type, const f32 *value)
{
    if (chan != NULL) {
        switch (type) {
        case 5:
            chan->mat_color.r = 255.0 * *value;
            return;
        case 6:
            chan->mat_color.g = 255.0 * *value;
            return;
        case 7:
            chan->mat_color.b = 255.0 * *value;
            return;
        case 8:
            chan->mat_color.a = 255.0 * *value;
            return;
        case 9:
            chan->amb_color.r = 255.0 * *value;
            return;
        case 10:
            chan->amb_color.g = 255.0 * *value;
            return;
        case 11:
            chan->amb_color.b = 255.0 * *value;
            return;
        case 12:
            chan->amb_color.a = 255.0 * *value;
            break;
        }
    }
}
#pragma pop

void _HSD_StateInvalidateColorChannel(void)
{
    static HSD_Chan invalid_prev_ch[4] = {
        {NULL, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
        {NULL, 1, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
        {NULL, 2, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL},
        {NULL, 3, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, 255, 0, 0, 0, 0, 2, NULL}};

    memcpy(&prev_ch, &invalid_prev_ch, sizeof(prev_ch));
    prev_amb_invalid[0] = 1;
    prev_amb_invalid[1] = 1;
    prev_mat_invalid[0] = 1;
    prev_mat_invalid[1] = 1;
    prev_num_chans = -1;
}

void _HSD_StateInvalidateTevStage(void)
{
    current_tev = 0;
}

void _HSD_StateInvalidateTevRegister(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        TevReg[i].c = 0;
    }
}

void _HSD_StateInvalidateTexCoordGen(void)
{
    num_tex_gens = 0;
}
