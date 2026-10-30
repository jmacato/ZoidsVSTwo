/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/spline.c. */
#include "spline.h"

extern f32 sqrtf__Ff(f32 value);
#define ABS(value) ((value) < 0.0f ? -(value) : (value))

static inline f32 splArcLengthPolynomial(const f32 coefficients[5], f32 time)
{
    f32 square = time * time;
    f32 cube = square * time;
    f32 fourth_power = cube * time;
    f32 result = (coefficients[0] * fourth_power) + (coefficients[1] * cube) +
                 (coefficients[2] * square) + (coefficients[3] * time) +
                 coefficients[4];

    if ((result < 0.0F) && (result > -0.001F)) {
        result = 0.0F;
    }

    return sqrtf__Ff(result);
}

static inline f32 *segment_coefficients(HSD_Spline *spline, s32 segment_index)
{
    return spline->segment_polynomials[segment_index];
}

static inline f32 integrate_middle(const f32 coefficients[5], const f32 step, f32 time)
{
    f32 weighted_sum = 0.0F;
    s32 sample;
    for (sample = 2; sample <= 8; ++sample) {
        if (!(sample & 1)) {
            weighted_sum += 4.0F * splArcLengthPolynomial(coefficients, time);
        } else {
            weighted_sum += 2.0F * splArcLengthPolynomial(coefficients, time);
        }
        time += step;
    }
    return weighted_sum;
}

static inline f32 integration_step(f32 start, f32 midpoint)
{
    return (midpoint - start) / 8.0F;
}

f32 splArcLengthGetParameter(HSD_Spline *spline, f32 distance)
{
    s32 segment_index = 0;
    f32 start = 0.0F;
    f32 end = 1.0F;
    f32 result;

    if (distance <= 0.0F) {
        return start;
    }

    if (distance >= 1.0F) {
        return end;
    }

    while (spline->segment_lengths[segment_index + 1] < distance) {
        segment_index++;
    }

    switch (spline->type) {
    case SPLINE_LINEAR: {
        result = (distance - spline->segment_lengths[segment_index]) /
                 (spline->segment_lengths[segment_index + 1] -
                  spline->segment_lengths[segment_index]);
    } break;
    case SPLINE_BEZIER:
    case SPLINE_BSPLINE:
    case SPLINE_CARDINAL: {

        f32 remaining_length =
            spline->total_length * (distance - spline->segment_lengths[segment_index]);

        while (ABS(start - end) >= 0.00001F) {
            const f32 *coefficients = segment_coefficients(spline, segment_index);
            f32 step;
            f32 middle;
            f32 simpsons;

            result = (start + end) / 2.0F;
            step = integration_step(start, result);
            middle = integrate_middle(coefficients, step, start + step);
            simpsons = step *
                       (middle + splArcLengthPolynomial(coefficients, start) +
                        splArcLengthPolynomial(coefficients, result)) /
                       3.0F;
            if (remaining_length < (0.00001F + simpsons)) {
                end = result;
            } else {
                start = result;
                remaining_length -= simpsons;
            }
        }
    } break;
    }

    return (result + segment_index) / (spline->control_count - 1.0F);
}
