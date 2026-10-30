#include "math_support.h"

extern void sRotY(Vec3 *point, f32 cosine, f32 sine);
extern void sRotX(Vec3 *point, f32 cosine, f32 sine);

void sRotVecYX(Vec3 *direction, Vec3 *point)
{
    f32 first;
    f32 second;
    f32 length;
    f32 first_square;
    f32 second_square;

    first = direction->x;
    second = direction->z;
    first_square = first * first;
    second_square = second * second;
    length = square_root(first_square + second_square);
    if (length != 0.0f) {
        sRotY(point, second / length, first / length);
        sRotY(direction, second / length, first / length);
    }

    first = direction->y;
    second = direction->z;
    first_square = first * first;
    second_square = second * second;
    length = square_root(first_square + second_square);
    if (length != 0.0f) {
        sRotX(point, second / length, first / length);
    }
}
