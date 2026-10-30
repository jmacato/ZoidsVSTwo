/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common_Embedded/Math/Single_precision/exponentialsf.c.
 */
#include "runtime_math.h"
#pragma peephole off
extern "C" unsigned long __float_huge[];
extern "C" const float __two_to_log2e_m1_tI[178];
extern "C" float expf(float);
extern "C" unsigned long __float_nan[];
extern "C" const float __one_over_F[129];

static float __log2_F[] = {
    -0.375F, -0.36377275F, -0.352632195F, -0.341576993F, -0.330605894F,
    -0.319717556F, -0.308910817F, -0.298184395F, -0.287537158F, -0.276967913F,
    -0.266475528F, -0.256058931F, -0.245716989F, -0.235448644F, -0.225252882F,
    -0.21512866F, -0.205074996F, -0.195090905F, -0.185175434F, -0.175327659F,
    -0.165546641F, -0.155831486F, -0.146181315F, -0.136595264F, -0.127072483F,
    -0.117612153F, -0.108213462F, -0.0988755971F, -0.089597784F, -0.0803792477F,
    -0.0712192506F, -0.0621170439F, -0.0530719049F, -0.0440831222F, -0.0351499952F,
    -0.0262718461F, -0.017447995F, -0.00867778528F, 3.94313465e-05F, 0.00870429259F,
    0.0173174236F, 0.0258794371F, 0.0343909375F, 0.0428525135F, 0.0512647554F,
    0.0596282259F, 0.0679434985F, 0.0762111098F, 0.0844316185F, 0.0926055536F,
    0.100733429F, 0.108815774F, 0.116853096F, 0.124845885F, 0.132794634F,
    0.140699834F, 0.148561954F, 0.156381458F, 0.164158806F, 0.171894461F,
    0.179588854F, 0.187242419F, 0.194855601F, 0.202428833F, 0.209962502F,
    0.217457041F, 0.224912837F, 0.232330307F, 0.239709839F, 0.24705182F,
    0.254356623F, 0.261624634F, 0.268856198F, 0.2760517F, 0.28321147F,
    0.290335923F, 0.29742533F, 0.304480106F, 0.31150052F, 0.318486959F,
    0.325439721F, 0.332359135F, 0.339245528F, 0.346099198F, 0.352920443F,
    0.35970962F, 0.366466999F, 0.373192847F, 0.379887491F, 0.386551231F,
    0.393184334F, 0.399787068F, 0.406359702F, 0.412902564F, 0.419415861F,
    0.425899893F, 0.432354927F, 0.438781202F, 0.445178956F, 0.451548487F,
    0.457890004F, 0.464203775F, 0.470490038F, 0.476749033F, 0.482980996F,
    0.489186138F, 0.495364726F, 0.501516938F, 0.507643044F, 0.513743222F,
    0.51981777F, 0.525866807F, 0.531890571F, 0.537889361F, 0.543863237F,
    0.549812496F, 0.555737317F, 0.561637938F, 0.567514479F, 0.573367238F,
    0.579196334F, 0.585001945F, 0.590784311F, 0.59654355F, 0.602279902F,
    0.607993603F, 0.613684714F, 0.619353414F, 0.625F,
};

static float __two_to_x[] = {
    0.693147182F, 0.240226507F, 0.0555041581F, 0.00961813424F, 0.00133318256F,
    0.00015401977F, 1.54832742e-05F, 1.33928177e-06F, 1.02999984e-07F,
};

inline float __log2f(float x)
{
    static float __log2e_m1[2] = { 0.41015625f, 0.03253879088896f };

    float frac;
    int exp, index;
    int hi_bits;
    float coefficients[2] = { -0.72135162353515625f, 0.4808933f };
    float c = 1.375f;

    exp   = (*(unsigned int*)&x >> 23) - 0x80;
    index = ((*(unsigned int*)&x) & 0x7FFFFF) >> 16;
    if ((unsigned short)(*(unsigned int*)&x) != 0) {
        hi_bits = ((*(unsigned int*)&x) & 0x7F0000) | 0x3F800000;
        int lo_bits = ((*(unsigned int*)&x) & 0x7FFFFF) | 0x3F800000;

        if ((*(unsigned int*)&x) & 0x8000) {
            index++;
            hi_bits += 0x10000;
        }

        frac = *(float*)&lo_bits - *(float*)&hi_bits;
        frac *= __one_over_F[index];

        return (float)exp + c
            + (__log2_F[index]
               + (frac
                  + ((__log2e_m1[0] * frac)
                     + ((__log2e_m1[1] * frac)
                        + (frac * frac
                           * ((frac * coefficients[1]) + coefficients[0]))))));
    } else {
        return (float)exp + c + __log2_F[index];
    }
}

inline float __exp2f(float x)
{
    int n;
    float f, p;

    n = (int)x;
    f = x - (float)n;
    if (n > 128)
        return *(float *)__float_huge;
    if (n < -127)
        return 0.0f;
    n = n + 127;
    n = n << 23;
    p = f * (f * (f * (f * (f * (f * (f * (f * (f * __two_to_x[8] + __two_to_x[7]) + __two_to_x[6]) + __two_to_x[5]) + __two_to_x[4]) + __two_to_x[3]) + __two_to_x[2]) + __two_to_x[1]) + __two_to_x[0]);
    x = 1.0f;
    return (p + x) * *(float *)&n;
}

inline int __fpclassifyf(float x)
{
    switch ((*(int *)&x) & 0x7F800000) {
    case 0x7F800000:
        if ((*(int *)&x) & 0x7FFFFF)
            return FP_QNAN;
        return FP_INFINITE;
    case 0:
        if ((*(int *)&x) & 0x7FFFFF)
            return FP_SUBNORMAL;
        return FP_ZERO;
    }
    return FP_NORMAL;
}

extern "C" float powf(float x, float y);

float powf(float x, float y)
{
    int ix;

    if (x > 0.0f)
        return __exp2f(y * __log2f(x));
    if (x < 0.0f) {
        ix = (int)y;
        if (y - (float)(int)y)
            return *(float *)__float_nan;
        if ((ix % 2) != 0)
            return -__exp2f(y * __log2f(-x));
        return __exp2f(y * __log2f(-x));
    }
    if (__fpclassifyf(x) == FP_QNAN)
        return x;

    switch (__fpclassifyf(y)) {
    case FP_ZERO:
        return 1.0f;
    case FP_QNAN:
    case FP_INFINITE:
        return *(float *)__float_nan;
    case FP_NORMAL:
    case FP_SUBNORMAL:
        if (*(unsigned int *)&x & 0x80000000)
            return *(float *)__float_huge;
        return x;
    default:
        break;
    }
    return 0.0f;
}

float expf(float x)
{
    static float __exp_to_x[] = {
        0.999999881F, 0.49999997F, 0.166667983F, 0.0416668877F,
        0.00832859613F, 0.001388276F, 0.000204699929F, 2.54991846e-05F,
    };
    float one = 1.0f;
    int integer;
    int index;
    float fraction;
    float power;
    float result;
    if (x > 88.72284f)
        return *(float *)__float_huge;
    if (x < -87.33655f)
        return 0.0f;
    integer = (int)x;
    index = integer + 88;
    __HI(power) = (index + 39) << 23;
    fraction = x - integer;
    result = fraction * (fraction * (fraction * (fraction * (fraction * (fraction *
        (fraction * (fraction * (__exp_to_x[7]) + __exp_to_x[6]) + __exp_to_x[5]) +
        __exp_to_x[4]) + __exp_to_x[3]) + __exp_to_x[2]) + __exp_to_x[1]) +
        __exp_to_x[0]);
    return __two_to_log2e_m1_tI[index] * (power * (result + one));
}
