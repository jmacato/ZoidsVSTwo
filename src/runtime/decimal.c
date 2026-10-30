/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common_Embedded/ansi_fp.c.
 */
#include "runtime_decimal.h"
#define FP_NAN 1
#define FP_QNAN FP_NAN
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5
#define __HI(value) (*(int *)&(value))
#define __LO(value) (*((int *)&(value) + 1))
#define isfinite(value) (__fpclassifyd(value) > FP_INFINITE)
#define isnan(value) (__fpclassifyd(value) == FP_NAN)
extern double frexp(double, int *);

static inline int __fpclassifyd(double __value)
{
    switch (__HI(__value) & 0x7ff00000) {
    case 0x7ff00000: {
        if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
            return FP_QNAN;
        else
            return FP_INFINITE;
        break;
    }
    case 0: {
        if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
            return FP_SUBNORMAL;
        else
            return FP_ZERO;
        break;
    }
    }
    return FP_NORMAL;
}

static const double bit_values[] = {
    1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256,
};

static const double digit_values[] = {
    1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8,
};

void __num2dec(const decform *f, double x, decimal *d)
{
    int binary_exponent;
    int exp;
    unsigned char *p;
    int digits;
    int power_exponent;
    double divisor;
    int digit_index;
    int chunk_digits;
    int padding_digits;
    int integer_digits;
    int padding_index;
    const double *power;

    digits = f->digits;
    if (digits > 16) {
        digits = 16;
    }

    d->sign = 0;
    d->exp = 0;
    d->sig.length = 1;

    if (x == 0.0) {
        d->sig.text[0] = '0';
        return;
    }

    if (!isfinite(x)) {
        d->sig.text[0] = isnan(x) ? 'N' : 'I';
        return;
    }

    d->sig.length = 0;
    if (x < 0.0) {
        x = -x;
        d->sign = 1;
    }

    frexp(x, &binary_exponent);
    power_exponent = (binary_exponent * 301029) / 1000000; // log_10(2)
    exp = power_exponent;
    power = bit_values;
    if (power_exponent < 0) {
        power_exponent = -power_exponent;
        while (power_exponent != 0) {
            if (power_exponent & 1) {
                x *= *power;
            }
            power_exponent >>= 1;
            power++;
        }
    } else if (power_exponent > 0) {
        divisor = 1.0f;
        while (power_exponent != 0) {
            if (power_exponent & 1) {
                divisor *= *power;
            }
            power_exponent >>= 1;
            power++;
        }
        x /= divisor;
    }

    while (x >= 1.0) {
        x *= 0.1;
        exp++;
    }

    while (x < 0.1) {
        x *= 10.0;
        exp--;
    }

    p = d->sig.text;
    while (digits != 0) {
        chunk_digits = digits;
        if (digits > 8) {
            chunk_digits = 8;
        }
        d->sig.length += chunk_digits;
        digits -= chunk_digits;
        exp -= chunk_digits;
        p += chunk_digits;
        x *= digit_values[chunk_digits - 1];
        integer_digits = (int)x;
        x = x - integer_digits;

        digit_index = chunk_digits + 1;
        while (--digit_index != 0) {
            *--p = '0' + (integer_digits % 10);
            integer_digits /= 10;
        }
        p += chunk_digits;
    }

    digits = f->digits;
    if (f->digits > SIGDIGLEN) {
        digits = SIGDIGLEN;
    }

    padding_digits = digits - d->sig.length;
    if (padding_digits > 0) {
        padding_index = padding_digits + 1;
        while (--padding_index != 0) {
            *p++ = '0';
        }
        exp -= padding_digits;
        d->sig.length += padding_digits;
    }

    d->exp = exp;
}
