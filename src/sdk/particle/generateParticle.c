#include "particle.h"
#include "cobj.h"
#include "random.h"

#define M_PI 3.14159265358979323846
#define M_PI_2 (M_PI / 2)
#define HSD_ASSERT(line, cond) ((cond) ? ((void)0) : __assert("generator.c", line, #cond))

extern void __assert(const char *file, u32 line, const char *condition);
extern f64 __frsqrte(f64);
extern f32 atan2f(f32, f32);
extern f32 sinf(f32);
extern f32 cosf(f32);
extern void PSMTXIdentity(Mtx m);
extern void PSMTXCopy(Mtx src, Mtx dst);
extern void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
extern void PSMTXMultVec(Mtx m, Vec3 *src, Vec3 *dst);
extern void PSVECNormalize(Vec3 *src, Vec3 *dst);
extern void PSVECCrossProduct(Vec3 *a, Vec3 *b, Vec3 *axb);
extern HSD_CObj *psCamera;
extern void (*psExtExecGenFunc)(HSD_Generator *generator, Mtx matrix);

static inline f32 fabsf_bitwise(f32 value)
{
    *(u32 *)&value &= 0x7FFFFFFF;
    return value;
}

static inline f32 sqrtf(f32 x)
{
    volatile f32 y;
    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        y = (f32)(x * guess);
        return y;
    }
    return x;
}

f32 generateParticle(HSD_Generator* gen)
{
    f32 r0;
    f32 mag;
    Vec3 vel_copy;
    f32 tmp;
    Vec3 emit_pos;
    Vec3 tmpvec;
    Vec3 vec;
    Mtx rot_mtx;
    Mtx jobj_mtx;
    Vec3 look_dir;
    Vec3 cam_up;
    Vec3 cross1;
    Vec3 vel_norm;
    Mtx trig_mtx;
    f32 vel_mag;
    f32 angle1;
    f32 sin_az;
    f32 cos_az;
    f32 sin_el;
    f32 angle_step;
    f32 cur_angle;
    f32 azimuth;
    f32 elevation;
    f32 polar;
    f32 angle3;
    f32 rand_radius;
    f32 emit_radius;
    f32 emit_angle;
    angle3 = angle1 = 0.0F;
    if (gen->count < 1.0F) {
        return gen->count;
    }

    vel_copy.x = gen->vel.x;
    vel_copy.y = gen->vel.y;
    vel_copy.z = gen->vel.z;

    vel_mag = sqrtf(vel_copy.z * vel_copy.z +
                       (vel_copy.x * vel_copy.x + vel_copy.y * vel_copy.y));

    PSMTXIdentity(rot_mtx);

    if ((gen->type & PS_GEN_JOBJ) && gen->jobj != 0) {
        PSMTXCopy(gen->jobj->matrix, jobj_mtx);
        if (!(gen->kind & 0x30000)) {

        vec.x = jobj_mtx[0][0];
        vec.y = jobj_mtx[1][0];
        vec.z = jobj_mtx[2][0];
        PSVECNormalize(&vec, &vec);
        rot_mtx[0][0] = vec.x;
        rot_mtx[1][0] = vec.y;
        rot_mtx[2][0] = vec.z;

        vec.x = jobj_mtx[0][1];
        vec.y = jobj_mtx[1][1];
        vec.z = jobj_mtx[2][1];
        PSVECNormalize(&vec, &vec);
        rot_mtx[0][1] = vec.x;
        rot_mtx[1][1] = vec.y;
        rot_mtx[2][1] = vec.z;

        vec.x = jobj_mtx[0][2];
        vec.y = jobj_mtx[1][2];
        vec.z = jobj_mtx[2][2];
        PSVECNormalize(&vec, &vec);
        rot_mtx[0][2] = vec.x;
        rot_mtx[1][2] = vec.y;
        rot_mtx[2][2] = vec.z;

        rot_mtx[2][3] = 0.0F;
        rot_mtx[1][3] = 0.0F;
        rot_mtx[0][3] = 0.0F;
        }
    }

    if (gen->kind & 0x10000) {
        HSD_ASSERT(516, psCamera);
        look_dir.x = psCamera->eyepos->position.x - gen->pos.x;
        look_dir.y = psCamera->eyepos->position.y - gen->pos.y;
        look_dir.z = psCamera->eyepos->position.z - gen->pos.z;
        PSVECNormalize(&look_dir, &look_dir);
        HSD_CObjGetUpVector(psCamera, &cam_up);
        PSVECNormalize(&cam_up, &cam_up);
        PSVECCrossProduct(&cam_up, &look_dir, &cross1);
        PSVECCrossProduct(&look_dir, &cross1, &cam_up);

        rot_mtx[0][0] = cross1.x;
        rot_mtx[1][0] = cross1.y;
        rot_mtx[2][0] = cross1.z;
        rot_mtx[0][1] = cam_up.x;
        rot_mtx[1][1] = cam_up.y;
        rot_mtx[2][1] = cam_up.z;
        rot_mtx[0][2] = look_dir.x;
        rot_mtx[1][2] = look_dir.y;
        rot_mtx[2][2] = look_dir.z;
    }

    if ((gen->type & PS_GEN_TYPE_MASK) != PS_GEN_LINE && vel_mag > 1.1920929e-7F) {
        vel_norm.x = gen->vel.x;
        vel_norm.y = gen->vel.y;
        vel_norm.z = gen->vel.z;
        PSVECNormalize(&vel_norm, &vel_norm);

        if (fabsf_bitwise(vel_norm.z) < 1.1754944e-38F) {
            if (vel_norm.y >= 0.0F) {
                sin_az = 1.5707964F;
            } else {
                sin_az = -1.5707964F;
            }
        } else {
            sin_az = atan2f(vel_norm.y, vel_norm.z);
        }
        cos_az = sinf(sin_az);
        sin_az = cosf(sin_az);

        {
            f32 projected_z = vel_norm.y * cos_az + vel_norm.z * sin_az;
            if (fabsf_bitwise(projected_z) < 1.1754944e-38F) {
                if (vel_norm.x >= 0.0F) {
                    elevation = 1.5707964F;
                } else {
                    elevation = -1.5707964F;
                }
            } else {
                elevation = atan2f(vel_norm.x, projected_z);
            }
        }
        sin_el = sinf(elevation);
        {
            f32 cos_el = cosf(elevation);
            f32 neg_cos_az = -cos_az;

            trig_mtx[0][0] = cos_el;
            trig_mtx[0][1] = 0.0F;
            trig_mtx[0][2] = sin_el;
            trig_mtx[0][3] = 0.0F;
            trig_mtx[1][0] = neg_cos_az * sin_el;
            trig_mtx[1][1] = sin_az;
            trig_mtx[1][2] = cos_az * cos_el;
            trig_mtx[1][3] = 0.0F;
            trig_mtx[2][0] = -sin_az * sin_el;
            trig_mtx[2][1] = neg_cos_az;
            trig_mtx[2][2] = sin_az * cos_el;
            trig_mtx[2][3] = 0.0F;
        }
        PSMTXConcat(rot_mtx, trig_mtx, rot_mtx);
    }
    elevation = vel_mag;

    if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_TORNADO) {
        if (fabsf_bitwise(rot_mtx[2][2]) < 1.1754944e-38F) {
            if (rot_mtx[1][2] >= 0.0F) {
                angle1 = 1.5707964F;
            } else {
                angle1 = -1.5707964F;
            }
        } else {
            angle1 = atan2f(rot_mtx[1][2], rot_mtx[2][2]);
        }
        {
            f32 first = rot_mtx[2][2] * cosf(angle1);
            f32 comb = first + rot_mtx[1][2] * sinf(angle1);
            if (fabsf_bitwise(comb) < 1.1754944e-38F) {
                if (rot_mtx[0][2] >= 0.0F) {
                    angle3 = 1.5707964F;
                } else {
                    angle3 = -1.5707964F;
                }
            } else {
                angle3 = atan2f(rot_mtx[0][2], comb);
            }
        }
    }

    if (gen->angle < 0.0F) {
        switch (gen->type & PS_GEN_TYPE_MASK) {
        case PS_GEN_DISC:
        case PS_GEN_DISC_3:
        case PS_GEN_DISC_4: {
            f32 min_a = gen->aux.disc.minAngle;
            f32 rnd = HSD_Randf();
            angle_step =
                (gen->aux.disc.maxAngle - min_a) / (f32) (s32) gen->count;
            cur_angle = angle_step * rnd + min_a;
            break;
        }
        case PS_GEN_CONE:
        case PS_GEN_CYLINDER: {
            f32 min_a = gen->aux.cone.minAngle;
            f32 rnd = HSD_Randf();
            angle_step =
                (gen->aux.cone.maxAngle - min_a) / (f32) (s32) gen->count;
            cur_angle = angle_step * rnd + min_a;
            break;
        }
        default: {
            cur_angle = (f32) (2.0 * (M_PI * HSD_Randf()));
            angle_step = (f32) ((2.0 * M_PI) / (f64) (s32) gen->count);
            break;
        }
        }
    }

    while (gen->count >= 1.0F) {
        switch (gen->type & PS_GEN_TYPE_MASK) {
        case PS_GEN_DISC:        case PS_GEN_DISC_3:
        case PS_GEN_DISC_4:
        case PS_GEN_CONE:
        case PS_GEN_CYLINDER: {
            if (gen->radius < 0.0F) {
                emit_radius = -gen->radius;
                rand_radius = 1.0F;
            } else {
                rand_radius = HSD_Randf();
                if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_DISC_3 || (gen->type & PS_GEN_TYPE_MASK) == PS_GEN_DISC_4) {
                    rand_radius = sqrtf(rand_radius);
                }
                emit_radius = rand_radius * gen->radius;
            }

            switch (gen->type & PS_GEN_TYPE_MASK) {
            case PS_GEN_CONE:
                if (gen->angle < 0.0F) {
                    tmp = cur_angle + angle_step;
                    cur_angle = tmp;
                    if (fabsf_bitwise(emit_radius) < 1.1754944e-38F) {
                        if (gen->aux.cone.height >= 0.0F) {
                            emit_angle = -gen->angle;
                        } else {
                            emit_angle = (f32) (M_PI - gen->angle);
                        }
                    } else {
                        emit_angle =
                            (f32) (M_PI_2 -
                                   atan2f(gen->aux.cone.height, emit_radius) -
                                   gen->angle);
                    }
                } else {
                    {
                        f32 min_angle = gen->aux.cone.minAngle;
                        cur_angle = min_angle;
                    }
                    {
                        f32 rnd = HSD_Randf();
                        emit_angle = gen->aux.cone.maxAngle - cur_angle;
                        cur_angle = emit_angle * rnd + cur_angle;
                        emit_angle = rand_radius * gen->angle;
                    }
                    if (fabsf_bitwise(emit_radius) < 1.1754944e-38F) {
                        if (gen->aux.cone.height >= 0.0F) {
                            emit_angle = gen->angle;
                        } else {
                            emit_angle = (f32) (M_PI + gen->angle);
                        }
                    } else {
                        emit_angle =
                            (f32) (gen->angle +
                                   (M_PI_2 -
                                    atan2f(gen->aux.cone.height, emit_radius)));
                    }
                }
                break;
            case PS_GEN_CYLINDER:
                if (gen->angle < 0.0F) {
                    emit_angle = (f32) (M_PI_2 - gen->angle);
                    tmp = cur_angle + angle_step;
                    cur_angle = tmp;
                } else {
                    {
                        f32 min_angle = gen->aux.cone.minAngle;
                        cur_angle = min_angle;
                    }
                    {
                        f32 rnd = HSD_Randf();
                        emit_angle = gen->aux.cone.maxAngle - cur_angle;
                        cur_angle = emit_angle * rnd + cur_angle;
                        emit_angle = (f32) (M_PI_2 + gen->angle);
                    }
                }
                break;
            default:
                if (gen->angle < 0.0F) {
                    emit_angle = -gen->angle;
                    cur_angle += angle_step;
                    emit_angle = rand_radius * emit_angle;
                } else {
                    {
                        f32 min_angle = gen->aux.disc.minAngle;
                        cur_angle = min_angle;
                    }
                    {
                        f32 rnd = HSD_Randf();
                        emit_angle = gen->aux.disc.maxAngle - cur_angle;
                        cur_angle = emit_angle * rnd + cur_angle;
                        emit_angle = rand_radius * gen->angle;
                    }
                }
                break;
            }

            emit_pos.x = emit_radius * cosf(cur_angle);
            emit_pos.y = emit_radius * sinf(cur_angle);

            if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_CONE || (gen->type & PS_GEN_TYPE_MASK) == PS_GEN_CYLINDER) {
                emit_pos.z = HSD_Randf();
                if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_CONE) {
                    emit_pos.x *= (1.0F - emit_pos.z);
                    emit_pos.y *= (1.0F - emit_pos.z);
                }
                emit_pos.z *= gen->aux.cone.height;
            } else {
                emit_pos.z = 0.0F;
            }

            {
                emit_radius = elevation * sinf(emit_angle);
                vec.x = emit_radius * cosf(cur_angle);
                vec.y = emit_radius * sinf(cur_angle);
                vec.z = elevation * cosf(emit_angle);
            }

            if ((gen->type & PS_GEN_TYPE_MASK) == PS_GEN_DISC_3) {
                vec.x *= rand_radius;
                vec.y *= rand_radius;
                vec.z *= rand_radius;
            }

            PSMTXMultVec(rot_mtx, &emit_pos, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            PSMTXMultVec(rot_mtx, &vec, &vec);

            psGenerateParticle(gen->linkNo, gen->bank, gen->kind, gen->texGroup, gen->cmdList, gen->life, 0, emit_pos.x, emit_pos.y, emit_pos.z, vec.x, vec.y, vec.z, gen->size, gen->grav, gen->fric, gen);
            break;
        }

        case PS_GEN_LINE:        {
            f32 rnd = HSD_Randf();
            emit_pos.x = rnd * gen->aux.line.x2;
            emit_pos.y = rnd * gen->aux.line.y2;
            emit_pos.z = rnd * gen->aux.line.z2;

            PSMTXMultVec(rot_mtx, &emit_pos, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            PSMTXMultVec(rot_mtx, &vel_copy, &vec);

            psGenerateParticle(gen->linkNo, gen->bank, gen->kind, gen->texGroup, gen->cmdList, gen->life, 0, emit_pos.x, emit_pos.y, emit_pos.z, vec.x, vec.y, vec.z, gen->size, gen->grav, gen->fric, gen);
            break;
        }

        case PS_GEN_TORNADO:        {
            if (gen->radius < 0.0F) {
                sin_az = 1.0F;
            } else {
                sin_az = HSD_Randf();
            }
            if (gen->angle < 0.0F) {
                cur_angle += angle_step;
            } else {
                f32 rnd = HSD_Randf();
                cur_angle =
                    (f32) (2.0 * (M_PI * (f64) rnd));
            }
            gen->aux.tornado.vel = elevation;
            psGenerateParticle(gen->linkNo, gen->bank, gen->kind | Tornado, gen->texGroup, gen->cmdList, gen->life, 0, 0.0F, 0.0F, 0.0F, cur_angle, sin_az, 0.0F, gen->size, angle1, angle3, gen);
            break;
        }

        case PS_GEN_RECT:        {
            emit_pos.x = HSD_Randf();
            emit_pos.y = HSD_Randf();
            emit_pos.z = HSD_Randf();

            switch (gen->aux.rect.flag) {
            case 0:                break;
            case 1:                emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                break;
            case 2:                emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                break;
            case 3:            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.x;
                if (rnd > a2 / (a2 + gen->aux.rect.y)) {
                    emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 4:                emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                break;
            case 5:            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.x;
                if (rnd > a2 / (a2 + gen->aux.rect.z)) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 6:            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.y;
                if (rnd > a2 / (a2 + gen->aux.rect.z)) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 7:            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.z;
                f32 b2 = gen->aux.rect.y;
                f32 c2 = gen->aux.rect.x;
                r0 = 1.0F / (c2 * (b2 + a2) + b2 * a2);
                if (rnd < r0 * (c2 * b2)) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else {
                    if (rnd > 1.0F - r0 * (c2 * a2)) {
                        emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                    } else {
                        emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                    }
                }
                break;
            }
            default:
                break;
            }

            emit_pos.x -= 0.5F;
            emit_pos.y -= 0.5F;
            emit_pos.z -= 0.5F;

            tmpvec.x = gen->aux.rect.xx * emit_pos.x +
                       gen->aux.rect.yx * emit_pos.y +
                       gen->aux.rect.zx * emit_pos.z;
            tmpvec.y = gen->aux.rect.xy * emit_pos.x +
                       gen->aux.rect.yy * emit_pos.y +
                       gen->aux.rect.zy * emit_pos.z;
            tmpvec.z = gen->aux.rect.xz * emit_pos.x +
                       gen->aux.rect.yz * emit_pos.y +
                       gen->aux.rect.zz * emit_pos.z;

            PSMTXMultVec(rot_mtx, &tmpvec, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            {
                mag = sqrtf(gen->aux.rect.zx * gen->aux.rect.zx +
                                gen->aux.rect.zy * gen->aux.rect.zy +
                                gen->aux.rect.zz * gen->aux.rect.zz);
                {
                    f32 scale = elevation / mag;
                    vec.x = gen->aux.rect.zx * scale;
                    vec.y = gen->aux.rect.zy * scale;
                    vec.z = gen->aux.rect.zz * scale;
                }
            }

            PSMTXMultVec(rot_mtx, &vec, &vec);

            psGenerateParticle(gen->linkNo, gen->bank, gen->kind, gen->texGroup, gen->cmdList, gen->life, 0, emit_pos.x, emit_pos.y, emit_pos.z, vec.x, vec.y, vec.z, gen->size, gen->grav, gen->fric, gen);
            break;
        }

        case PS_GEN_SPHERE:        {
            r0 = gen->aux.sphere.latRange;
            if (r0 == 0.0F) {
                polar = HSD_Randf();
                polar = sqrtf(polar);
                polar = (f32) (M_PI * polar);
            } else {
                polar = HSD_Randf();
                polar = sqrtf(polar);
                polar = gen->aux.sphere.latRange * polar;
            }
            {
                azimuth = HSD_Randf();
                azimuth =
                    (f32) (2.0 * (M_PI * (f64) azimuth));
                {
                    f32 r2 = gen->radius;
                    if (r2 < 0.0F) {
                        sin_az = -r2;
                    } else {
                        sin_az = HSD_Randf();
                        sin_az = sqrtf(sin_az);
                        sin_az = r2 * sin_az;
                    }
                }
            }
            {
                f32 cos_r;
                f32 sin_r;
                cos_r = cosf(azimuth);
                cos_az = sinf(polar);
                vec.x = cos_az * cos_r;
                sin_el = sinf(azimuth);
                sin_r = sinf(polar);
                vec.y = sin_r * sin_el;
                vec.z = cosf(polar);
            }

            PSMTXMultVec(rot_mtx, &vec, &emit_pos);

            vec.x = emit_pos.x * gen->aux.sphere.speed;
            vec.y = emit_pos.y * gen->aux.sphere.speed;
            vec.z = emit_pos.z * gen->aux.sphere.speed;

            if (gen->radius >= 0.0F && gen->aux.sphere.speed < 0.0F) {
                f32 scale = sin_az / gen->radius;
                vec.x *= scale;
                vec.y *= scale;
                vec.z *= scale;
            }

            emit_pos.x = sin_az * emit_pos.x + gen->pos.x;
            emit_pos.y = sin_az * emit_pos.y + gen->pos.y;
            emit_pos.z = sin_az * emit_pos.z + gen->pos.z;

            psGenerateParticle(gen->linkNo, gen->bank, gen->kind, gen->texGroup, gen->cmdList, gen->life, 0, emit_pos.x, emit_pos.y, emit_pos.z, vec.x, vec.y, vec.z, gen->size, gen->grav, gen->fric, gen);
            break;
        }

        default:            if (psExtExecGenFunc != 0) {
                psExtExecGenFunc(gen, rot_mtx);
            }
            break;
        }

        gen->count -= 1.0F;
    }

    return gen->count;
}
