/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/texpdag.c. */
#include "texp.h"
#include "gx_tev_api.h"
static int SimplifySrc(HSD_TExp *expression);
static int SimplifyThis(HSD_TExp *expression);
static int SimplifyByMerge(HSD_TExp *expression);

#pragma push
/* Keep the original calls to these helpers. */
#pragma dont_inline on
static int CalcBias(GXTevOp operation, GXTevBias bias, GXTevBias source_bias,
                    GXTevBias *result)
{
    switch (source_bias) {
    case GX_TB_ADDHALF:
        source_bias = 1;
        break;
    case GX_TB_SUBHALF:
        source_bias = -1;
        break;
    default:
        source_bias = 0;
        break;
    }
    if (operation == GX_TEV_SUB)
        source_bias = -source_bias;
    switch (bias) {
    case GX_TB_ADDHALF:
        source_bias += 1;
        break;
    case GX_TB_SUBHALF:
        source_bias -= 1;
        break;
    }
    switch (source_bias) {
    case 0:
        *result = GX_TB_ZERO;
        return 1;
    case 1:
        *result = GX_TB_ADDHALF;
        return 1;
    case -1:
        *result = GX_TB_SUBHALF;
        return 1;
    }
    return 0;
}

static int GXTevScale2Value(GXTevScale scale)
{
    switch (scale) {
    case GX_CS_SCALE_1:
        return 2;
    case GX_CS_SCALE_2:
        return 4;
    case GX_CS_SCALE_4:
        return 8;
    case GX_CS_DIVIDE_2:
        return 1;
    default:
        return 2;
    }
}

#pragma pop
static void CalcTevRange(HSD_TExp *expression)
{
    int range = 0;
    int i;
    if (expression->tev.c_in[3].sel != HSD_TE_0) {
        switch (HSD_TExpGetType(expression->tev.c_in[3].exp)) {
        case HSD_TE_TEV:
            if (expression->tev.c_in[3].sel == HSD_TE_RGB) {
                if (expression->tev.c_in[3].exp->tev.c_clamp != 1 &&
                    expression->tev.c_in[3].exp->tev.c_range == 1) {
                    expression->tev.c_range = 1;
                    goto alpha;
                }
                range = 256;
            } else {
                if (expression->tev.c_in[3].exp->tev.a_clamp != 1 &&
                    expression->tev.c_in[3].exp->tev.a_range == 1) {
                    expression->tev.c_range = 1;
                    goto alpha;
                }
                range = 256;
            }
            break;
        default:
            range = 256;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (expression->tev.c_in[i].sel != HSD_TE_0) {
            if (expression->tev.c_op == GX_TEV_SUB) {
                expression->tev.c_range = 1;
                goto alpha;
            }
            range += 256;
            break;
        }
    }
    switch (expression->tev.c_bias) {
    case GX_TB_ADDHALF:
        range += 128;
        break;
    case GX_TB_SUBHALF:
        expression->tev.c_range = 1;
        goto alpha;
    }
    switch (expression->tev.c_scale) {
    case GX_CS_SCALE_2:
        range *= 2;
        break;
    case GX_CS_SCALE_4:
        range *= 4;
        break;
    case GX_CS_DIVIDE_2:
        range /= 2;
        break;
    }
    expression->tev.c_range = range > 256 ? 1 : 0;
alpha:
    range = 0;
    if (expression->tev.a_in[3].sel != HSD_TE_0) {
        switch (HSD_TExpGetType(expression->tev.a_in[3].exp)) {
        case HSD_TE_TEV:
            if (expression->tev.a_in[3].exp->tev.a_clamp != 1 &&
                expression->tev.a_in[3].exp->tev.a_range == 1) {
                expression->tev.a_range = 1;
                return;
            }
            range = 256;
            break;
        default:
            range = 256;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (expression->tev.a_in[i].sel != HSD_TE_0) {
            if (expression->tev.a_op == GX_TEV_SUB) {
                expression->tev.a_range = 1;
                return;
            }
            range += 256;
            break;
        }
    }
    switch (expression->tev.a_bias) {
    case GX_TB_ADDHALF:
        range += 128;
        break;
    case GX_TB_SUBHALF:
        expression->tev.a_range = 1;
        return;
    }
    switch (expression->tev.a_scale) {
    case GX_CS_SCALE_2:
        range *= 2;
        break;
    case GX_CS_SCALE_4:
        range *= 4;
        break;
    case GX_CS_DIVIDE_2:
        range /= 2;
        break;
    }
    expression->tev.a_range = range > 256 ? 1 : 0;
}

#pragma push
/* The recursive pass calls this function without inlining it. */
#pragma dont_inline on
int HSD_TExpSimplify(HSD_TExp *expression)
{
    HSD_TExp *texp = expression;
    int result = 0;
    if (HSD_TExpGetType(texp) != HSD_TE_TEV)
        return 0;
    if (SimplifySrc(texp))
        result = 1;
    if (SimplifyThis(texp))
        result = 1;
    if (SimplifyByMerge(texp))
        result = 1;
    CalcTevRange(texp);
    return result;
}

#pragma pop
static HSD_TEArg TE_ARG_ZERO = {HSD_TE_ZERO, HSD_TE_0, HSD_TE_UNDEF};

static int SimplifySrc(HSD_TExp *expression)
{
    int i;
    int result;

    result = 0;
    i = 0;

    for (; i < 4; i++) {
        if (expression->tev.c_in[i].type == HSD_TE_TEV) {
            HSD_TExp *src = expression->tev.c_in[i].exp;
            u8 sel = expression->tev.c_in[i].sel;
            if (HSD_TExpSimplify(src) != 0) {
                result = 1;
            }
            if (sel == HSD_TE_RGB) {
                switch (src->tev.c_op) {
                case HSD_TE_UNDEF:
                    HSD_TExpUnref(src, sel);
                    result = 1;
                    expression->tev.c_in[i] = TE_ARG_ZERO;
                    break;
                case GX_TEV_ADD:
                    if (src->tev.c_in[0].sel == HSD_TE_0 &&
                        src->tev.c_in[1].sel == HSD_TE_0 && src->tev.c_bias == GX_TB_ZERO &&
                        src->tev.c_scale == GX_CS_SCALE_1) {
                        switch (src->tev.c_in[3].type) {
                        case HSD_TE_IMM:
                            expression->tev.c_in[i] = src->tev.c_in[3];
                            HSD_TExpRef(expression->tev.c_in[i].exp,
                                        expression->tev.c_in[i].sel);
                            HSD_TExpUnref(src, sel);
                            result = 1;
                            break;
                        case HSD_TE_TEV:
                            if (i != 3 || src->tev.c_clamp == 0 ||
                                src->tev.c_range != 1) {
                                expression->tev.c_in[i] = src->tev.c_in[3];
                                HSD_TExpRef(expression->tev.c_in[i].exp,
                                            expression->tev.c_in[i].sel);
                                HSD_TExpUnref(src, sel);
                                result = 1;
                            }
                            break;
                        case HSD_TE_TEX:
                            if ((expression->tev.tex == 0 ||
                                 expression->tev.tex == src->tev.tex) &&
                                (expression->tev.tex_swap == HSD_TE_UNDEF ||
                                 src->tev.tex_swap == HSD_TE_UNDEF ||
                                 expression->tev.tex_swap == src->tev.tex_swap)) {
                                expression->tev.c_in[i] = src->tev.c_in[3];
                                expression->tev.tex = src->tev.tex;
                                if (expression->tev.tex_swap == HSD_TE_UNDEF) {
                                    expression->tev.tex_swap = src->tev.tex_swap;
                                }
                                HSD_TExpUnref(src, sel);
                                result = 1;
                            }
                            break;
                        case HSD_TE_RAS:
                            if ((expression->tev.chan == HSD_TE_UNDEF ||
                                 expression->tev.chan == src->tev.chan) &&
                                (expression->tev.ras_swap == HSD_TE_UNDEF ||
                                 src->tev.ras_swap == HSD_TE_UNDEF ||
                                 expression->tev.ras_swap == src->tev.ras_swap)) {
                                expression->tev.c_in[i] = src->tev.c_in[3];
                                expression->tev.chan = src->tev.chan;
                                if (expression->tev.ras_swap == HSD_TE_UNDEF) {
                                    expression->tev.ras_swap = src->tev.ras_swap;
                                }
                                HSD_TExpUnref(src, sel);
                                result = 1;
                            }
                            break;
                        }
                    }
                    break;
                }
            } else {
                switch (src->tev.a_op) {
                case GX_TEV_ADD:
                    break;
                case HSD_TE_UNDEF:
                    HSD_TExpUnref(src, sel);
                    result = 1;
                    expression->tev.c_in[i] = TE_ARG_ZERO;
                    break;
                }
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if (expression->tev.a_in[i].type == HSD_TE_TEV) {
            HSD_TExp *src = expression->tev.a_in[i].exp;
            u8 sel = expression->tev.a_in[i].sel;
            HSD_TExpSimplify(src);
            switch (src->tev.a_op) {
            case HSD_TE_UNDEF:
                HSD_TExpUnref(src, sel);
                result = 1;
                expression->tev.a_in[i] = TE_ARG_ZERO;
                break;
            case GX_TEV_ADD:
                if (src->tev.a_in[0].sel == HSD_TE_0 &&
                    src->tev.a_in[1].sel == HSD_TE_0 && src->tev.a_bias == GX_TB_ZERO &&
                    src->tev.a_scale == GX_CS_SCALE_1) {
                    switch (src->tev.a_in[3].type) {
                    case HSD_TE_IMM:
                        expression->tev.a_in[i] = src->tev.a_in[3];
                        HSD_TExpRef(expression->tev.a_in[i].exp,
                                    expression->tev.a_in[i].sel);
                        HSD_TExpUnref(src, sel);
                        result = 1;
                        /* Preserve the original SDK fallthrough. */
                    case HSD_TE_TEV:
                        if (i != 3 || src->tev.a_clamp == 0 || src->tev.a_range != 1) {
                            expression->tev.a_in[i] = src->tev.a_in[3];
                            HSD_TExpRef(expression->tev.a_in[i].exp,
                                        expression->tev.a_in[i].sel);
                            HSD_TExpUnref(src, sel);
                            result = 1;
                        }
                        break;
                    case HSD_TE_TEX:
                        if (expression->tev.tex == 0 ||
                            expression->tev.tex == src->tev.tex) {
                            expression->tev.a_in[i] = src->tev.a_in[3];
                            expression->tev.tex = src->tev.tex;
                            HSD_TExpUnref(src, sel);
                            result = 1;
                        }
                        break;
                    case HSD_TE_RAS:
                        if (expression->tev.chan == HSD_TE_UNDEF ||
                            expression->tev.chan == src->tev.chan) {
                            expression->tev.a_in[i] = src->tev.a_in[3];
                            expression->tev.chan = src->tev.chan;
                            HSD_TExpUnref(src, sel);
                            result = 1;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    return result;
}

#define CLEAR_ARG(arg)                                                                 \
    do {                                                                               \
        (arg) = TE_ARG_ZERO;                                                           \
    } while (0)

static int SimplifyThis(HSD_TExp *expression)
{
    int color_tex;
    int alpha_tex;
    int color_ras;
    int alpha_ras;
    int result;
    int changed;
    int op;
    int i;

    result = 0;
    do {
        color_tex = -1;
        alpha_tex = -1;
        color_ras = -1;
        alpha_ras = -1;

        for (i = 0; i < 4; i++) {
            switch (expression->tev.c_in[i].type) {
            case HSD_TE_TEX:
                color_tex = i;
                break;
            case HSD_TE_RAS:
                color_ras = i;
                break;
            }
            switch (expression->tev.a_in[i].type) {
            case HSD_TE_TEX:
                alpha_tex = i;
                break;
            case HSD_TE_RAS:
                alpha_ras = i;
                break;
            }
        }

        if (color_tex == -1 && alpha_tex == -1) {
            expression->tev.tex = 0;
            expression->tev.tex_swap = HSD_TE_UNDEF;
        }
        if (color_ras == -1 && alpha_ras == -1) {
            expression->tev.chan = HSD_TE_UNDEF;
            expression->tev.ras_swap = HSD_TE_UNDEF;
        }

        changed = 0;
        if (expression->tev.a_op == HSD_TE_UNDEF || expression->tev.a_op == GX_TEV_COMP_A8_GT ||
            expression->tev.a_op == GX_TEV_COMP_A8_EQ || expression->tev.a_op <= GX_TEV_SUB) {
            if (expression->tev.c_op != HSD_TE_UNDEF && expression->tev.c_ref == 0) {
                expression->tev.c_op = HSD_TE_UNDEF;
                for (i = 0; i < 4; i++) {
                    HSD_TExpUnref(expression->tev.c_in[i].exp,
                                  expression->tev.c_in[i].sel);
                    expression->tev.c_in[i] = TE_ARG_ZERO;
                }
                changed = 1;
            }

            switch (expression->tev.c_op) {
            case GX_TEV_ADD:
            case GX_TEV_SUB:
                if (expression->tev.c_in[2].sel == HSD_TE_0) {
                    if (expression->tev.c_in[1].sel != HSD_TE_0) {
                        HSD_TExpUnref(expression->tev.c_in[1].exp,
                                      expression->tev.c_in[1].sel);
                        changed = 1;
                        CLEAR_ARG(expression->tev.c_in[1]);
                    }
                    if (expression->tev.c_op == GX_TEV_ADD &&
                        expression->tev.c_in[3].sel == HSD_TE_0) {
                        changed = 1;
                        expression->tev.c_in[3] = expression->tev.c_in[0];
                        CLEAR_ARG(expression->tev.c_in[0]);
                        expression->tev.c_clamp = 1;
                    }
                }
                if (expression->tev.c_in[2].sel == HSD_TE_1) {
                    if (expression->tev.c_in[0].sel != HSD_TE_0) {
                        HSD_TExpUnref(expression->tev.c_in[0].exp,
                                      expression->tev.c_in[0].sel);
                        changed = 1;
                        CLEAR_ARG(expression->tev.c_in[0]);
                    }
                    if (expression->tev.c_op == GX_TEV_ADD &&
                        expression->tev.c_in[3].sel == HSD_TE_0) {
                        changed = 1;
                        expression->tev.c_in[3] = expression->tev.c_in[1];
                        CLEAR_ARG(expression->tev.c_in[1]);
                        CLEAR_ARG(expression->tev.c_in[2]);
                    }
                }
                if (expression->tev.c_in[0].sel == HSD_TE_0 &&
                    expression->tev.c_in[1].sel == HSD_TE_1) {
                    changed = 1;
                    expression->tev.c_in[0] = expression->tev.c_in[2];
                    CLEAR_ARG(expression->tev.c_in[1]);
                    CLEAR_ARG(expression->tev.c_in[2]);
                }
                if (expression->tev.c_in[0].sel == HSD_TE_0 &&
                    expression->tev.c_in[1].sel == HSD_TE_0 &&
                    expression->tev.c_in[3].sel == HSD_TE_0 &&
                    expression->tev.c_bias == GX_TB_ZERO) {
                    expression->tev.c_op = HSD_TE_UNDEF;
                    HSD_TExpUnref(expression->tev.c_in[2].exp,
                                  expression->tev.c_in[2].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.c_in[2]);
                }
                break;
            case GX_TEV_COMP_R8_GT:
            case GX_TEV_COMP_GR16_GT:
            case GX_TEV_COMP_BGR24_GT:
            case GX_TEV_COMP_RGB8_GT:
                if (expression->tev.c_in[2].sel == HSD_TE_0) {
                    expression->tev.c_op = GX_TEV_ADD;
                    HSD_TExpUnref(expression->tev.c_in[0].exp,
                                  expression->tev.c_in[0].sel);
                    CLEAR_ARG(expression->tev.c_in[0]);
                    HSD_TExpUnref(expression->tev.c_in[1].exp,
                                  expression->tev.c_in[1].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.c_in[1]);
                } else if (expression->tev.c_in[0].sel == HSD_TE_0) {
                    expression->tev.c_op = GX_TEV_ADD;
                    HSD_TExpUnref(expression->tev.c_in[1].exp,
                                  expression->tev.c_in[1].sel);
                    CLEAR_ARG(expression->tev.c_in[1]);
                    HSD_TExpUnref(expression->tev.c_in[2].exp,
                                  expression->tev.c_in[2].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.c_in[2]);
                }
                break;
            case GX_TEV_COMP_R8_EQ:
            case GX_TEV_COMP_GR16_EQ:
            case GX_TEV_COMP_BGR24_EQ:
            case GX_TEV_COMP_RGB8_EQ:
                if (expression->tev.c_in[2].sel == HSD_TE_0) {
                    expression->tev.c_op = GX_TEV_ADD;
                    HSD_TExpUnref(expression->tev.c_in[0].exp,
                                  expression->tev.c_in[0].sel);
                    CLEAR_ARG(expression->tev.c_in[0]);
                    HSD_TExpUnref(expression->tev.c_in[1].exp,
                                  expression->tev.c_in[1].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.c_in[1]);
                } else if (expression->tev.c_in[0].sel == HSD_TE_0 &&
                           expression->tev.c_in[1].sel == HSD_TE_0) {
                    expression->tev.c_op = GX_TEV_ADD;
                    changed = 1;
                    expression->tev.c_in[0] = expression->tev.c_in[2];
                    CLEAR_ARG(expression->tev.c_in[2]);
                }
                break;
            }
        }

        if (expression->tev.a_op != HSD_TE_UNDEF && expression->tev.a_ref == 0) {
            expression->tev.a_op = HSD_TE_UNDEF;
            for (i = 0; i < 4; i++) {
                HSD_TExpUnref(expression->tev.a_in[i].exp, expression->tev.a_in[i].sel);
                CLEAR_ARG(expression->tev.a_in[i]);
            }
            changed = 1;
        }

        switch (expression->tev.a_op) {
        case GX_TEV_ADD:
        case GX_TEV_SUB:
            if (expression->tev.a_in[2].sel == HSD_TE_0) {
                if (expression->tev.a_in[1].sel != HSD_TE_0) {
                    HSD_TExpUnref(expression->tev.a_in[1].exp,
                                  expression->tev.a_in[1].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.a_in[1]);
                }
                if (expression->tev.a_op == GX_TEV_ADD &&
                    expression->tev.a_in[3].sel == HSD_TE_0) {
                    changed = 1;
                    expression->tev.a_in[3] = expression->tev.a_in[0];
                    CLEAR_ARG(expression->tev.a_in[0]);
                }
            }
            if (expression->tev.a_in[2].sel == HSD_TE_1) {
                if (expression->tev.a_in[0].sel != HSD_TE_0) {
                    HSD_TExpUnref(expression->tev.a_in[0].exp,
                                  expression->tev.a_in[0].sel);
                    changed = 1;
                    CLEAR_ARG(expression->tev.a_in[0]);
                }
                if (expression->tev.a_op == GX_TEV_ADD &&
                    expression->tev.a_in[3].sel == HSD_TE_0) {
                    changed = 1;
                    expression->tev.a_in[3] = expression->tev.a_in[1];
                    CLEAR_ARG(expression->tev.a_in[1]);
                    CLEAR_ARG(expression->tev.a_in[2]);
                }
            }
            if (expression->tev.a_in[0].sel == HSD_TE_0 &&
                expression->tev.a_in[1].sel == HSD_TE_0 &&
                expression->tev.a_in[3].sel == HSD_TE_0) {
                expression->tev.a_op = HSD_TE_UNDEF;
                changed = 1;
            }
            break;

        case GX_TEV_COMP_R8_GT:
        case GX_TEV_COMP_R8_EQ:
        case GX_TEV_COMP_GR16_GT:
        case GX_TEV_COMP_GR16_EQ:
        case GX_TEV_COMP_BGR24_GT:
        case GX_TEV_COMP_BGR24_EQ:
            if (expression->tev.a_in[2].sel == HSD_TE_0) {
                expression->tev.a_op = GX_TEV_ADD;
                HSD_TExpUnref(expression->tev.a_in[0].exp, expression->tev.a_in[0].sel);
                CLEAR_ARG(expression->tev.a_in[0]);
                HSD_TExpUnref(expression->tev.a_in[1].exp, expression->tev.a_in[1].sel);
                changed = 1;
                CLEAR_ARG(expression->tev.a_in[1]);
            }
            break;

        case GX_TEV_COMP_A8_GT:
            if (expression->tev.a_in[2].sel == HSD_TE_0) {
                expression->tev.a_op = GX_TEV_ADD;
                HSD_TExpUnref(expression->tev.a_in[0].exp, expression->tev.a_in[0].sel);
                CLEAR_ARG(expression->tev.a_in[0]);
                HSD_TExpUnref(expression->tev.a_in[1].exp, expression->tev.a_in[1].sel);
                changed = 1;
                CLEAR_ARG(expression->tev.a_in[1]);
            } else if (expression->tev.a_in[0].sel == HSD_TE_0) {
                expression->tev.a_op = GX_TEV_ADD;
                HSD_TExpUnref(expression->tev.a_in[1].exp, expression->tev.a_in[1].sel);
                CLEAR_ARG(expression->tev.a_in[1]);
                HSD_TExpUnref(expression->tev.a_in[2].exp, expression->tev.a_in[2].sel);
                changed = 1;
                CLEAR_ARG(expression->tev.a_in[2]);
            }
            break;

        case GX_TEV_COMP_A8_EQ:
            if (expression->tev.a_in[2].sel == HSD_TE_0) {
                expression->tev.a_op = GX_TEV_ADD;
                HSD_TExpUnref(expression->tev.a_in[0].exp, expression->tev.a_in[0].sel);
                CLEAR_ARG(expression->tev.a_in[0]);
                HSD_TExpUnref(expression->tev.a_in[1].exp, expression->tev.a_in[1].sel);
                changed = 1;
                CLEAR_ARG(expression->tev.a_in[1]);
            } else if (expression->tev.a_in[0].sel == HSD_TE_0 &&
                       expression->tev.a_in[1].sel == HSD_TE_0) {
                expression->tev.a_op = GX_TEV_ADD;
                changed = 1;
                expression->tev.a_in[0] = expression->tev.a_in[2];
                CLEAR_ARG(expression->tev.a_in[2]);
            }
            break;
        }

        if (changed != 0) {
            result = 1;
        }
    } while (changed != 0);

    return result;
}

static inline s32 IsThroughColor(HSD_TExp *texp)
{
    return texp->tev.c_op == GX_TEV_ADD && texp->tev.c_in[0].sel == HSD_TE_0 &&
           texp->tev.c_in[1].sel == HSD_TE_0 && texp->tev.c_bias == GX_TB_ZERO &&
           texp->tev.c_scale == GX_CS_SCALE_1;
}
static inline s32 IsThroughAlpha(HSD_TExp *texp)
{
    return texp->tev.a_op == GX_TEV_ADD && texp->tev.a_in[0].sel == HSD_TE_0 &&
           texp->tev.a_in[1].sel == HSD_TE_0 && texp->tev.a_bias == GX_TB_ZERO &&
           texp->tev.a_scale == GX_CS_SCALE_1;
}

int HSD_TExpSimplify2(HSD_TExp *texp_)
{
    HSD_TExp *texp = (HSD_TExp *)texp_;
    HSD_TExp *src_exp;
    u8 src_sel;
    int i;

    for (i = 0; i < 4; i++) {
        src_exp = texp->tev.c_in[i].exp;
        src_sel = texp->tev.c_in[i].sel;
        if (texp->tev.c_in[i].type == HSD_TE_TEV && src_sel == HSD_TE_RGB &&
            IsThroughColor(src_exp)) {
            switch (src_exp->tev.c_in[3].type) {
            case HSD_TE_KONST:
                if (texp->tev.kcsel == HSD_TE_UNDEF) {
                    texp->tev.kcsel = src_exp->tev.kcsel;
                } else if (texp->tev.kcsel != src_exp->tev.kcsel) {
                    break;
                }
                /* fallthrough */
            case HSD_TE_IMM:
                texp->tev.c_in[i] = src_exp->tev.c_in[3];
                HSD_TExpRef(texp->tev.c_in[i].exp, texp->tev.c_in[i].sel);
                HSD_TExpUnref(src_exp, src_sel);
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        src_exp = texp->tev.a_in[i].exp;
        src_sel = texp->tev.a_in[i].sel;
        if (texp->tev.a_in[i].type == HSD_TE_TEV && IsThroughAlpha(src_exp)) {
            switch (src_exp->tev.a_in[3].type) {
            case HSD_TE_KONST:
                if (texp->tev.kasel == HSD_TE_UNDEF) {
                    texp->tev.kasel = src_exp->tev.kasel;
                } else if (texp->tev.kasel != src_exp->tev.kasel) {
                    break;
                }
                /* fallthrough */
            case HSD_TE_IMM:
                texp->tev.a_in[i] = src_exp->tev.a_in[3];
                HSD_TExpRef(texp->tev.a_in[i].exp, texp->tev.a_in[i].sel);
                HSD_TExpUnref(src_exp, src_sel);
                break;
            }
        }
    }
    return 0;
}

static inline int ResourceConflict(HSD_TExp *parent, HSD_TExp *source)
{
    if (parent->tev.tex != 0 && source->tev.tex != 0 &&
        parent->tev.tex != source->tev.tex)
        return 1;
    else if (parent->tev.chan != HSD_TE_UNDEF && source->tev.chan != HSD_TE_UNDEF &&
             parent->tev.chan != source->tev.chan)
        return 1;
    else
        return 0;
}
/* These macros preserve the original SDK stack layout. */
#define HasColorConstant(expression)                                                   \
    ((expression)->tev.c_in[0].type == HSD_TE_CNST ||                                  \
     (expression)->tev.c_in[1].type == HSD_TE_CNST ||                                  \
     (expression)->tev.c_in[2].type == HSD_TE_CNST ||                                  \
     (expression)->tev.c_in[3].type == HSD_TE_CNST)
#define HasAlphaConstant(expression)                                                   \
    ((expression)->tev.a_in[0].type == HSD_TE_CNST ||                                  \
     (expression)->tev.a_in[1].type == HSD_TE_CNST ||                                  \
     (expression)->tev.a_in[2].type == HSD_TE_CNST ||                                  \
     (expression)->tev.a_in[3].type == HSD_TE_CNST)
static inline int MergeScale(GXTevScale parent, GXTevScale child, GXTevScale *scale)
{
    switch (GXTevScale2Value(parent) * GXTevScale2Value(child) / 2) {
    case 1:
        *scale = GX_CS_DIVIDE_2;
        break;
    case 2:
        *scale = GX_CS_SCALE_1;
        break;
    case 4:
        *scale = GX_CS_SCALE_2;
        break;
    case 8:
        *scale = GX_CS_SCALE_4;
        break;
    default:
        return 0;
    }
    return 1;
}

static int SimplifyByMerge(HSD_TExp *expression)
{
    HSD_TExp *child;
    u8 child_selector;
    int result = 0;
    int merged;
    int bias;
    GXTevScale scale;
    int i;
    do {
        merged = 0;

        if ((expression->tev.c_op == GX_TEV_ADD || expression->tev.c_op == GX_TEV_SUB) &&
            expression->tev.c_in[2].sel == HSD_TE_0) {
            child = expression->tev.c_in[0].exp;
            child_selector = expression->tev.c_in[0].sel;
            if (expression->tev.c_in[0].type == HSD_TE_TEV &&
                expression->tev.c_in[0].sel == HSD_TE_RGB && child->tev.c_range == 0 &&
                child->tev.c_in[3].sel == HSD_TE_0 &&
                ResourceConflict(expression, child) == 0 &&
                (HasColorConstant(expression) == 0 || HasColorConstant(child) == 0)) {
                goto merge_c_inputs;
            }
            child = expression->tev.c_in[3].exp;
            child_selector = expression->tev.c_in[3].sel;
            if (expression->tev.c_op == GX_TEV_ADD &&
                expression->tev.c_in[3].type == HSD_TE_TEV &&
                expression->tev.c_in[3].sel == HSD_TE_RGB && child->tev.c_range == 0 &&
                child->tev.c_in[3].sel == HSD_TE_0 &&
                ResourceConflict(expression, child) == 0 &&
                (HasColorConstant(expression) == 0 || HasColorConstant(child) == 0)) {
                HSD_TEArg input;
                if (expression->tev.c_in[0].type == HSD_TE_TEV) {
                    if (expression->tev.c_in[0].sel == HSD_TE_RGB) {
                        if (expression->tev.c_in[0].exp->tev.c_range != 0 &&
                            expression->tev.c_in[0].exp->tev.c_clamp != 1)
                            goto next_color;
                    } else {
                        if (expression->tev.c_in[0].exp->tev.a_range != 0 &&
                            expression->tev.c_in[0].exp->tev.a_clamp != 1)
                            goto next_color;
                    }
                }
                input = expression->tev.c_in[0];
                expression->tev.c_in[0] = expression->tev.c_in[3];
                expression->tev.c_in[3] = input;
            merge_c_inputs:
                if (child->tev.c_scale == GX_CS_SCALE_1) {
                    if (CalcBias(expression->tev.c_op, expression->tev.c_bias,
                                 child->tev.c_bias, &bias) == 0)
                        goto next_color;
                    scale = expression->tev.c_scale;
                } else {
                    if (expression->tev.c_in[3].sel != HSD_TE_0 ||
                        expression->tev.c_bias != GX_TB_ZERO ||
                        MergeScale(expression->tev.c_scale, child->tev.c_scale,
                                   &scale) == 0)
                        goto next_color;
                    bias = child->tev.c_bias;
                }
                if (expression->tev.c_op == GX_TEV_SUB)
                    expression->tev.c_op = child->tev.c_op == GX_TEV_ADD ? GX_TEV_SUB : GX_TEV_ADD;
                else
                    expression->tev.c_op = child->tev.c_op;
                for (i = 0; i < 3; i++) {
                    expression->tev.c_in[i] = child->tev.c_in[i];
                    HSD_TExpRef(expression->tev.c_in[i].exp,
                                expression->tev.c_in[i].sel);
                }
                expression->tev.c_bias = bias;
                expression->tev.c_scale = scale;
                HSD_TExpUnref(child, child_selector);

                if (expression->tev.tex == 0)
                    expression->tev.tex = child->tev.tex;
                if (expression->tev.chan == HSD_TE_UNDEF)
                    expression->tev.chan = child->tev.chan;
                if (expression->tev.tex_swap == HSD_TE_UNDEF)
                    expression->tev.tex_swap = child->tev.tex_swap;
                if (expression->tev.ras_swap == HSD_TE_UNDEF)
                    expression->tev.ras_swap = child->tev.ras_swap;

                merged = 1;
            }
        }
    next_color:

        child = expression->tev.c_in[3].exp;
        child_selector = expression->tev.c_in[3].sel;
        if (expression->tev.c_in[3].type == HSD_TE_TEV &&
            child_selector == HSD_TE_RGB && ResourceConflict(expression, child) == 0 &&
            (HasColorConstant(expression) == 0 || HasColorConstant(child) == 0)) {
            if (child->tev.c_range != 0 && child->tev.c_clamp == 1) {
                if (expression->tev.c_in[0].sel == HSD_TE_0 &&
                    expression->tev.c_in[2].sel == HSD_TE_0 &&
                    expression->tev.c_bias == GX_TB_ZERO && expression->tev.c_scale != GX_CS_DIVIDE_2 &&
                    MergeScale(expression->tev.c_scale, child->tev.c_scale, &scale)) {
                    merged = 1;
                    expression->tev.c_bias = child->tev.c_bias;
                    expression->tev.c_scale = scale;
                    if (expression->tev.c_clamp != 1)
                        expression->tev.c_clamp = child->tev.c_clamp;

                    if (expression->tev.tex == 0)
                        expression->tev.tex = child->tev.tex;
                    if (expression->tev.chan == HSD_TE_UNDEF)
                        expression->tev.chan = child->tev.chan;
                    if (expression->tev.tex_swap == HSD_TE_UNDEF)
                        expression->tev.tex_swap = child->tev.tex_swap;
                    if (expression->tev.ras_swap == HSD_TE_UNDEF)
                        expression->tev.ras_swap = child->tev.ras_swap;

                    for (i = 0; i < 4; i++) {
                        HSD_TExp *previous = expression->tev.c_in[i].exp;
                        u8 selector = expression->tev.c_in[i].sel;
                        expression->tev.c_in[i] = child->tev.c_in[i];
                        HSD_TExpRef(expression->tev.c_in[i].exp,
                                    expression->tev.c_in[i].sel);
                        HSD_TExpUnref(previous, selector);
                    }
                }
            } else if (expression->tev.a_op == HSD_TE_UNDEF || expression->tev.a_op == GX_TEV_COMP_A8_GT ||
                       expression->tev.a_op == GX_TEV_COMP_A8_EQ || expression->tev.a_op <= GX_TEV_SUB ||
                       (child->tev.c_in[0].sel == HSD_TE_0 &&
                        child->tev.c_in[1].sel == HSD_TE_0 &&
                        child->tev.c_in[2].sel == HSD_TE_0)) {
                if (child->tev.c_scale == GX_CS_SCALE_1) {
                    if (((expression->tev.c_in[0].sel == HSD_TE_0 &&
                          expression->tev.c_in[2].sel == HSD_TE_0) ||
                         (child->tev.c_in[0].sel == HSD_TE_0 &&
                          child->tev.c_in[2].sel == HSD_TE_0)) &&
                        CalcBias(GX_TEV_ADD, expression->tev.c_bias, child->tev.c_bias, &bias)) {
                        scale = expression->tev.c_scale;
                        goto merge_c_output;
                    }
                } else if (expression->tev.c_in[0].sel == HSD_TE_0 &&
                           expression->tev.c_in[1].sel == HSD_TE_0 &&
                           expression->tev.c_in[2].sel == HSD_TE_0 &&
                           expression->tev.c_bias == GX_TB_ZERO &&
                           MergeScale(expression->tev.c_scale, child->tev.c_scale,
                                      &scale)) {
                    bias = child->tev.c_bias;
                merge_c_output:
                    merged = 1;
                    expression->tev.c_in[3] = child->tev.c_in[3];
                    HSD_TExpRef(expression->tev.c_in[3].exp,
                                expression->tev.c_in[3].sel);
                    expression->tev.c_bias = bias;
                    expression->tev.c_scale = scale;

                    if (expression->tev.tex == 0)
                        expression->tev.tex = child->tev.tex;
                    if (expression->tev.chan == HSD_TE_UNDEF)
                        expression->tev.chan = child->tev.chan;
                    if (expression->tev.tex_swap == HSD_TE_UNDEF)
                        expression->tev.tex_swap = child->tev.tex_swap;
                    if (expression->tev.ras_swap == HSD_TE_UNDEF)
                        expression->tev.ras_swap = child->tev.ras_swap;

                    if (child->tev.c_in[0].sel != HSD_TE_0 ||
                        child->tev.c_in[1].sel != HSD_TE_0 ||
                        child->tev.c_in[2].sel != HSD_TE_0) {
                        for (i = 0; i < 3; i++) {
                            HSD_TExp *previous = expression->tev.c_in[i].exp;
                            u8 selector = expression->tev.c_in[i].sel;
                            expression->tev.c_in[i] = child->tev.c_in[i];
                            HSD_TExpRef(expression->tev.c_in[i].exp,
                                        expression->tev.c_in[i].sel);
                            HSD_TExpUnref(previous, selector);
                        }
                    }
                    HSD_TExpUnref(child, child_selector);
                }
            }
        }

        if ((expression->tev.a_op == GX_TEV_ADD || expression->tev.a_op == GX_TEV_SUB) &&
            expression->tev.a_in[2].sel == HSD_TE_0) {
            child = expression->tev.a_in[0].exp;
            child_selector = expression->tev.a_in[0].sel;
            if (expression->tev.a_in[0].type == HSD_TE_TEV && child->tev.a_range == 0 &&
                child->tev.a_in[3].sel == HSD_TE_0 &&
                ResourceConflict(expression, child) == 0 &&
                (HasAlphaConstant(expression) == 0 || HasAlphaConstant(child) == 0)) {
                goto merge_a_inputs;
            }
            child = expression->tev.a_in[3].exp;
            child_selector = expression->tev.a_in[3].sel;
            /* The original SDK checks the child's constants twice here. */
            if (expression->tev.a_op == GX_TEV_ADD &&
                expression->tev.a_in[3].type == HSD_TE_TEV && child->tev.a_range == 0 &&
                child->tev.a_in[3].sel == HSD_TE_0 &&
                ResourceConflict(expression, child) == 0 &&
                (HasAlphaConstant(child) == 0 || HasAlphaConstant(child) == 0) &&
                (expression->tev.a_in[0].type != HSD_TE_TEV ||
                 expression->tev.a_in[0].exp->tev.a_range == 0 ||
                 expression->tev.a_in[0].exp->tev.a_clamp == 1)) {
                HSD_TEArg input = expression->tev.a_in[0];
                expression->tev.a_in[0] = expression->tev.a_in[3];
                expression->tev.a_in[3] = input;
            merge_a_inputs:
                if (child->tev.a_scale == GX_CS_SCALE_1) {
                    if (CalcBias(expression->tev.a_op, expression->tev.a_bias,
                                 child->tev.a_bias, &bias) == 0)
                        goto next_alpha;
                    scale = expression->tev.a_scale;
                } else {
                    if (expression->tev.a_in[3].sel != HSD_TE_0 ||
                        expression->tev.a_bias != GX_TB_ZERO ||
                        MergeScale(expression->tev.a_scale, child->tev.a_scale,
                                   &scale) == 0)
                        goto next_alpha;
                    bias = child->tev.a_bias;
                }
                if (expression->tev.a_op == GX_TEV_SUB)
                    expression->tev.a_op = child->tev.a_op == GX_TEV_ADD ? GX_TEV_SUB : GX_TEV_ADD;
                else
                    expression->tev.a_op = child->tev.a_op;
                for (i = 0; i < 3; i++) {
                    expression->tev.a_in[i] = child->tev.a_in[i];
                    HSD_TExpRef(expression->tev.a_in[i].exp,
                                expression->tev.a_in[i].sel);
                }
                expression->tev.a_bias = bias;
                expression->tev.a_scale = scale;
                HSD_TExpUnref(child, child_selector);

                if (expression->tev.tex == 0)
                    expression->tev.tex = child->tev.tex;
                if (expression->tev.chan == HSD_TE_UNDEF)
                    expression->tev.chan = child->tev.chan;
                if (expression->tev.tex_swap == HSD_TE_UNDEF)
                    expression->tev.tex_swap = child->tev.tex_swap;
                if (expression->tev.ras_swap == HSD_TE_UNDEF)
                    expression->tev.ras_swap = child->tev.ras_swap;

                merged = 1;
            }
        }
    next_alpha:

        child = expression->tev.a_in[3].exp;
        child_selector = expression->tev.a_in[3].sel;
        if (expression->tev.a_in[3].type == HSD_TE_TEV &&
            ResourceConflict(expression, child) == 0 &&
            (HasAlphaConstant(expression) == 0 || HasAlphaConstant(child) == 0)) {
            if (child->tev.a_range != 0 && child->tev.a_clamp == 1) {
                if (expression->tev.a_in[0].sel == HSD_TE_0 &&
                    expression->tev.a_in[2].sel == HSD_TE_0 &&
                    expression->tev.a_bias == GX_TB_ZERO && expression->tev.a_scale != GX_CS_DIVIDE_2 &&
                    MergeScale(expression->tev.a_scale, child->tev.a_scale, &scale)) {
                    merged = 1;
                    expression->tev.a_bias = child->tev.a_bias;
                    expression->tev.a_scale = scale;
                    if (expression->tev.a_clamp != 1)
                        expression->tev.a_clamp = child->tev.a_clamp;

                    if (expression->tev.tex == 0)
                        expression->tev.tex = child->tev.tex;
                    if (expression->tev.chan == HSD_TE_UNDEF)
                        expression->tev.chan = child->tev.chan;
                    if (expression->tev.tex_swap == HSD_TE_UNDEF)
                        expression->tev.tex_swap = child->tev.tex_swap;
                    if (expression->tev.ras_swap == HSD_TE_UNDEF)
                        expression->tev.ras_swap = child->tev.ras_swap;

                    for (i = 0; i < 4; i++) {
                        HSD_TExp *previous = expression->tev.a_in[i].exp;
                        u8 selector = expression->tev.a_in[i].sel;
                        expression->tev.a_in[i] = child->tev.a_in[i];
                        HSD_TExpRef(expression->tev.a_in[i].exp,
                                    expression->tev.a_in[i].sel);
                        HSD_TExpUnref(previous, selector);
                    }
                }
            } else {
                if (child->tev.a_scale == GX_CS_SCALE_1) {
                    if (((expression->tev.a_in[0].sel == HSD_TE_0 &&
                          expression->tev.a_in[2].sel == HSD_TE_0) ||
                         (child->tev.a_in[0].sel == HSD_TE_0 &&
                          child->tev.a_in[2].sel == HSD_TE_0)) &&
                        CalcBias(GX_TEV_ADD, expression->tev.a_bias, child->tev.a_bias, &bias)) {
                        scale = expression->tev.a_scale;
                        goto merge_a_output;
                    }
                } else if (expression->tev.a_in[0].sel == HSD_TE_0 &&
                           expression->tev.a_in[1].sel == HSD_TE_0 &&
                           expression->tev.a_in[2].sel == HSD_TE_0 &&
                           expression->tev.a_bias == GX_TB_ZERO &&
                           MergeScale(expression->tev.a_scale, child->tev.a_scale,
                                      &scale)) {
                    bias = child->tev.a_bias;
                merge_a_output:
                    merged = 1;
                    expression->tev.a_in[3] = child->tev.a_in[3];
                    HSD_TExpRef(expression->tev.a_in[3].exp,
                                expression->tev.a_in[3].sel);
                    expression->tev.a_bias = bias;
                    expression->tev.a_scale = scale;

                    if (expression->tev.tex == 0)
                        expression->tev.tex = child->tev.tex;
                    if (expression->tev.chan == HSD_TE_UNDEF)
                        expression->tev.chan = child->tev.chan;
                    if (expression->tev.tex_swap == HSD_TE_UNDEF)
                        expression->tev.tex_swap = child->tev.tex_swap;
                    if (expression->tev.ras_swap == HSD_TE_UNDEF)
                        expression->tev.ras_swap = child->tev.ras_swap;

                    if (child->tev.a_in[0].sel != HSD_TE_0 ||
                        child->tev.a_in[1].sel != HSD_TE_0 ||
                        child->tev.a_in[2].sel != HSD_TE_0) {
                        for (i = 0; i < 3; i++) {
                            HSD_TExp *previous = expression->tev.a_in[i].exp;
                            u8 selector = expression->tev.a_in[i].sel;
                            expression->tev.a_in[i] = child->tev.a_in[i];
                            HSD_TExpRef(expression->tev.a_in[i].exp,
                                        expression->tev.a_in[i].sel);
                            HSD_TExpUnref(previous, selector);
                        }
                    }
                    HSD_TExpUnref(child, child_selector);
                }
            }
        }

        if (merged)
            result = 1;
    } while (merged);
    return result;
}
