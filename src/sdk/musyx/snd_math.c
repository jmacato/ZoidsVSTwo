/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/snd_math.c (CC0-1.0). */
#include "musyx_spatial.h"
#include "constraint_math.h"
#pragma fp_contract off
#define sqrtf constraint_root
void salApplyMatrix(const SND_FMATRIX *mat, const SND_FVECTOR *in, SND_FVECTOR *out) {
  out->x = mat->m[0][0] * in->x + mat->m[0][1] * in->y + mat->m[0][2] * in->z + mat->t[0];
  out->y = mat->m[1][0] * in->x + mat->m[1][1] * in->y + mat->m[1][2] * in->z + mat->t[1];
  out->z = mat->m[2][0] * in->x + mat->m[2][1] * in->y + mat->m[2][2] * in->z + mat->t[2];
}

float salNormalizeVector(SND_FVECTOR *vec) {
  float l = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
  vec->x /= l;
  vec->y /= l;
  vec->z /= l;
  return l;
}
