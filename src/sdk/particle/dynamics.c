/* Direction math follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/particle.c. */
#include "appsrt.h"
#include "joint_matrix.h"
#include "random.h"
extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 atan2f(f32, f32);
extern f64 __frsqrte(f64);
#pragma push
#pragma force_active on
static void genPosUpdate(HSD_Generator *generator)
{
    HSD_JObj *joint;
    if (generator == 0)
        return;
    if (generator->kind & 0x800)
        return;
    if (!(generator->type & PS_GEN_JOBJ))
        return;
    if ((joint = generator->jobj) == 0)
        return;
    setup_matrix(joint);
    if ((generator->type & PS_GEN_APPSRT) && generator->appsrt != 0) {
        if (generator->appsrt->gp == generator) {
            generator->appsrt->translate.x = generator->jobj->matrix[0][3];
            generator->appsrt->translate.y = generator->jobj->matrix[1][3];
            generator->appsrt->translate.z = generator->jobj->matrix[2][3];
        }
    } else {
        generator->pos.x = generator->jobj->matrix[0][3];
        generator->pos.y = generator->jobj->matrix[1][3];
        generator->pos.z = generator->jobj->matrix[2][3];
    }
}
static void modifyDir(HSD_Particle *pp, f32 angle)
{
    f32 vx = pp->vel.x;
    f32 vz = pp->vel.z;
    f32 vy = pp->vel.y;
    f32 sin_a, cos_a;
    f32 sin_e, cos_e;
    f32 cos_rand, sin_rand;
    f32 rand_angle;
    f32 angle_copy;
    f32 sin_angle, cos_angle;
    f32 abs_z;
    f32 temp;
    f32 abs_temp;
    volatile f32 magnitude;
    s32 small_z, nonnegative_y, small_projection, nonnegative_x;

    {
        f32 azimuth;

        abs_z = vz;
        *(s32 *)&abs_z &= 0x7FFFFFFF;
        small_z = abs_z < 1.1754944e-38F;
        if (small_z) {
            nonnegative_y = vy >= 0.0F;
            if (nonnegative_y) {
                azimuth = 1.5707964F;
            } else {
                azimuth = -1.5707964F;
            }
        } else {
            azimuth = atan2f(vy, vz);
        }

        sin_a = sinf(azimuth);
        cos_a = cosf(azimuth);
    }

    {
        f32 azimuth;

        temp = vy * sin_a + vz * cos_a;
        abs_temp = temp;
        *(s32 *)&abs_temp &= 0x7FFFFFFF;
        small_projection = abs_temp < 1.1754944e-38F;
        if (small_projection) {
            nonnegative_x = vx >= 0.0F;
            if (nonnegative_x) {
                azimuth = 1.5707964F;
            } else {
                azimuth = -1.5707964F;
            }
        } else {
            azimuth = atan2f(vx, temp);
        }

        sin_e = sinf(azimuth);
        cos_e = cosf(azimuth);
    }

    vx = (vx * vx + vy * vy) + vz * vz;
    if (vx > 0.0F) {
        f64 x = __frsqrte((f64)vx);
        x = 0.5 * x * -(((f64)vx * (x * x)) - 3.0);
        x = 0.5 * x * -(((f64)vx * (x * x)) - 3.0);
        x = 0.5 * x * -(((f64)vx * (x * x)) - 3.0);
        magnitude = (f32)((f64)vx * x);
        vx = magnitude;
    }

    rand_angle = (f32)(3.141592653589793 * HSD_Randf() * 2.0);

    angle_copy = angle;
    sin_angle = vx * sinf(angle_copy);
    cos_rand = sin_angle * cosf(rand_angle);
    sin_rand = sin_angle * sinf(rand_angle);
    cos_angle = vx * cosf(angle_copy);

    pp->vel.x = cos_rand * cos_e + cos_angle * sin_e;
    pp->vel.y =
        cos_e * (cos_angle * sin_a) + (sin_e * (-cos_rand * sin_a) + sin_rand * cos_a);
    pp->vel.z =
        cos_e * (cos_angle * cos_a) + (sin_e * (-cos_rand * cos_a) - sin_rand * sin_a);
}

static s32 applyForceJObj(HSD_Particle *particle, HSD_JObj *joint, f32 force, f32 range)
{
    f32 dx, dy, dz, distance_squared, scale;
    if (joint == 0 || range < 0.0F)
        return 0;
    setup_matrix(joint);
    dx = joint->matrix[0][3] - particle->pos.x;
    dy = joint->matrix[1][3] - particle->pos.y;
    dz = joint->matrix[2][3] - particle->pos.z;
    distance_squared = dx * dx + dy * dy + dz * dz;
    if (distance_squared <= range * range)
        return 1;
    if (distance_squared == 0.0F)
        return 0;
    scale = force / distance_squared;
    particle->vel.x += scale * dx;
    particle->vel.y += scale * dy;
    particle->vel.z += scale * dz;
    return 0;
}
#pragma pop
