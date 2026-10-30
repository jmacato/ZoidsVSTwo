/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/texpdag.c. */
#include "texp.h"
extern void *memset(void *memory, s32 value, u32 size);
extern void __assert(const char *file, u32 line, const char *condition);
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if ((condition) == 0)                                                          \
            __assert("texpdag.c", line, #condition);                                   \
    } while (0)
int assign_reg(int num, u32 *dep, HSD_TExpDag *list, const int *order)
{
    u8 color_refs[4] = {0};
    u8 alpha_refs[4] = {0};
    HSD_TETev *tev;
    HSD_TExpDag *dag_entry;
    int idx;
    int i;
    int min_color_reg = 4;
    int min_alpha_reg = 4;
    int alpha_ref_count;
    int has_color_refs;

    for (idx = num - 1; idx >= 0; idx--) {
        dag_entry = &list[order[idx]];
        tev = dag_entry->tev;

        for (i = 0; i < 4; i++) {
            if (HSD_TExpGetType(tev->c_in[i].exp) == HSD_TE_TEV) {
                if (tev->c_in[i].sel == HSD_TE_RGB) {
                    color_refs[tev->c_in[i].exp->tev.c_dst] -= 1;
                } else {
                    alpha_refs[tev->c_in[i].exp->tev.a_dst] -= 1;
                }
            }
            if (HSD_TExpGetType(tev->a_in[i].exp) == HSD_TE_TEV) {
                alpha_refs[tev->a_in[i].exp->tev.a_dst] -= 1;
            }
        }

        tev = dag_entry->tev;
        has_color_refs = tev->c_ref > 0;
        if (has_color_refs) {
            for (i = 3; i >= 0; i--) {
                if (color_refs[i] == 0) {
                    color_refs[i] = tev->c_ref;
                    tev->c_dst = i;
                    if (min_color_reg > i) {
                        min_color_reg = i;
                    }
                    break;
                }
            }
        }
        alpha_ref_count = tev->a_ref;
        if (alpha_ref_count > 0) {
            for (i = 3; i >= 0; i--) {
                if (alpha_refs[i] == 0) {
                    alpha_refs[i] = alpha_ref_count;
                    tev->a_dst = i;
                    if (min_alpha_reg > i) {
                        min_alpha_reg = i;
                    }
                    break;
                }
            }
        }
    }
    return (4 - min_color_reg) + (4 - min_alpha_reg);
}

void order_dag(int num, u32 *dep, u32 *full_dep, HSD_TExpDag *list, int depth, int idx,
               u32 done_set, u32 ready_set, int *order, int *min, int *min_order)
{
    int *order_arg;
    int *min_arg;
    int *min_order_arg;
    HSD_TExpDag *dag;
    u32 dep_bits;
    u32 blocked;
    u32 full_bits;
    int score;
    int i;

    done_set |= 1 << idx;
    ready_set &= ~(1 << idx);
    order[depth++] = (u8)idx;

    if (depth == num) {
        score = assign_reg(num, dep, list, order);
        if (score < *min) {
            *min = score;
            for (i = 0; i < num; i++) {
                min_order[i] = order[i];
            }
        }
    } else {
        {
            dep_bits = dep[idx];
            blocked = ready_set | dep_bits;
            full_bits = 0;
            for (i = 0; i < num; i++) {
                if (blocked & (1 << i)) {
                    full_bits |= full_dep[i];
                }
            }
        }
        dag = &list[idx];
        ready_set = blocked & ~full_bits;
        if (dag->nb_dep == 1 && (ready_set & dep_bits)) {
            order_arg = order;
            min_arg = min;
            min_order_arg = min_order;
            order_dag(num, dep, full_dep, list, depth, dag->depend[0]->idx, done_set,
                      ready_set, order_arg, min_arg, min_order_arg);
        } else {
            for (i = 0; i < num; i++) {
                if (ready_set & (1 << i)) {
                    order_arg = order;
                    min_arg = min;
                    min_order_arg = min_order;
                    order_dag(num, dep, full_dep, list, depth, i, done_set, ready_set,
                              order_arg, min_arg, min_order_arg);
                }
            }
        }
    }
}

void CalcDistance(HSD_TExp **tevs, int *dist, HSD_TExp *tev, int num, int depth)
{
    HSD_TExp **p;
    int idx;
    int i;

    p = tevs;
    idx = 0;
    for (i = 0; i < num; i++) {
        if (*p == tev) {
            if (dist[idx] < depth) {
                dist[idx] = depth;
                for (i = 0; i < 4; i++) {
                    if (tev->tev.c_in[i].type == HSD_TE_TEV) {
                        CalcDistance(tevs, dist, tev->tev.c_in[i].exp, num, depth + 1);
                    }
                    if (tev->tev.a_in[i].type == HSD_TE_TEV) {
                        CalcDistance(tevs, dist, tev->tev.a_in[i].exp, num, depth + 1);
                    }
                }
            }
            return;
        }
        p++;
        idx++;
    }
}

#define HSD_TEXP_MAX_NUM 32

int HSD_TExpMakeDag(HSD_TExp *root, HSD_TExpDag *list)
{
    HSD_TExp *tevs[HSD_TEXP_MAX_NUM];
    int dist[32];
    int n;
    int j;
    HSD_TExp **base;
    int i;
    int k;
    int l;
    int idx;
    int num;
    HSD_TExpDag *dag;

    HSD_ASSERT(238, HSD_TExpGetType(root) == HSD_TE_TEV);

    base = tevs;
    n = 0;
    base[n++] = root;
    for (j = 0; j < n; j++) {
        HSD_TExp *tmp;
        if ((j < HSD_TEXP_MAX_NUM) == 0) {
            __assert("texpdag.c", 246, "j<HSD_TEXP_MAX_NUM");
        }
        tmp = tevs[j];
        for (i = 0; i < 4; i++) {
            if (tmp->tev.c_in[i].type == HSD_TE_TEV) {
                for (k = 0; k < n; k++) {
                    if (base[k] == tmp->tev.c_in[i].exp) {
                        break;
                    }
                }
                if (k >= n) {
                    base[n++] = tmp->tev.c_in[i].exp;
                }
            }
        }

        for (i = 0; i < 4; i++) {
            if (tmp->tev.a_in[i].type == HSD_TE_TEV) {
                for (k = 0; k < n; k++) {
                    if (base[k] == tmp->tev.a_in[i].exp) {
                        break;
                    }
                }
                if (k >= n) {
                    base[n++] = tmp->tev.a_in[i].exp;
                }
            }
        }
    }

    num = n;
    for (n = 0; n < num; n++) {
        dist[n] = -1;
    }

    CalcDistance(&tevs[0], &dist[0], tevs[0], num, 0);
    for (idx = 0; idx < num; idx++) {
        for (j = idx + 1; j < num; j++) {
            if (dist[j - 1] > dist[j]) {
                {
                    HSD_TExp *tmp = tevs[j - 1];
                    tevs[j - 1] = tevs[j];
                    tevs[j] = tmp;
                }

                {
                    int tmp = dist[j - 1];
                    dist[j - 1] = dist[j];
                    dist[j] = tmp;
                }
            }
        }
    }

    for (j = num - 1; j >= 0; j--) {
        HSD_TExp *tmp;
        dag = &list[j];
        tmp = tevs[j];
        dag->idx = (u8)j;
        dag->nb_ref = 0;
        dag->nb_dep = 0;
        dag->tev = &tmp->tev;
        for (idx = 0; idx < 4; idx++) {
            if (tmp->tev.c_in[idx].type == HSD_TE_TEV) {
                for (l = j; l < num; l++) {
                    if (tmp->tev.c_in[idx].exp == tevs[l]) {
                        HSD_TExpDag *dep_entry = &list[l];
                        for (l = 0; l < dag->nb_dep; l++) {
                            if (dag->depend[l] == dep_entry) {
                                break;
                            }
                        }
                        if (l >= (int)dag->nb_dep) {
                            dag->depend[dag->nb_dep++] = dep_entry;
                            dep_entry->nb_ref++;
                        }
                        break;
                    }
                }
                HSD_ASSERT(325, l < num);
            }
        }

        for (idx = 0; idx < 4; idx++) {
            if (tmp->tev.a_in[idx].type == HSD_TE_TEV) {
                for (l = j; l < num; l++) {
                    if (tmp->tev.a_in[idx].exp == tevs[l]) {
                        u8 dep_count2 = dag->nb_dep;
                        HSD_TExpDag *dep_entry2 = &list[l];
                        for (l = 0; l < (int)dep_count2; l++) {
                            if (dag->depend[l] == dep_entry2) {
                                break;
                            }
                        }
                        if (l >= dag->nb_dep) {
                            dag->depend[dag->nb_dep++] = dep_entry2;
                            dep_entry2->nb_ref++;
                        }
                        break;
                    }
                }
                HSD_ASSERT(347, l < num);
            }
        }
    }
    return num;
}

static inline void make_dependancy_mtx(int num, HSD_TExpDag *list, u32 *dep_mtx)
{
    HSD_TExpDag *dag;
    int i, j;

    for (i = 0; i < num; i++) {
        dep_mtx[i] = 0;
        dag = &list[i];
        for (j = 0; j < dag->nb_dep; j++) {
            dep_mtx[i] |= 1 << dag->depend[j]->idx;
        }
    }
}

void make_full_dependancy_mtx(int num, const u32 *dep, u32 *full)
{
    int i, j, k;
    s32 changed;
    u32 bits;
    u32 flag;
    u32 old;

    for (i = 0; i < num; i++) {
        full[i] = dep[i];
    }
    do {
        changed = 0;
        for (j = 0; j < num; j++) {
            flag = 1 << j;
            old = full[j];
            bits = old;
            for (k = 0; k < num; k++) {
                if ((flag & full[k]) != 0) {
                    u32 old = full[k];
                    full[k] |= bits;
                    if (old != full[k]) {
                        changed = 1;
                    }
                }
            }
        }
    } while (changed != 0);
}

void HSD_TExpSchedule(int num, HSD_TExpDag *list, HSD_TExp **result,
                      HSD_TExpRes *resource)
{

    u32 dep_mtx[32];
    u32 full_dep_matrix[32];
    u32 order[32];
    u32 min_order[32];

    int i, j;
    int min;

    min = 5;
    memset(min_order, 0, sizeof(min_order));
    make_dependancy_mtx(num, list, dep_mtx);
    make_full_dependancy_mtx(num, dep_mtx, full_dep_matrix);
    order_dag(num, dep_mtx, full_dep_matrix, list, 0, 0, 0, 0, (int *)order, &min,
              (int *)min_order);

    for (i = 0; i < num; i++) {
        result[i] = (HSD_TExp *)list[min_order[i]].tev;
        if (result[i]->tev.c_dst != HSD_TE_UNDEF) {
            resource->reg[result[i]->tev.c_dst + 4].color = 3;

            for (j = 0; j < 4; j++) {
                if (HSD_TExpGetType(result[i]->tev.c_in[j].exp) == HSD_TE_TEV) {
                    if (result[i]->tev.c_in[j].sel == HSD_TE_RGB) {
                        static int in[4] = {GX_CC_C0, GX_CC_C1, GX_CC_C2, GX_CC_CPREV};
                        result[i]->tev.c_in[j].arg =
                            in[result[i]->tev.c_in[j].exp->tev.c_dst];
                    } else {
                        static int in[4] = {GX_CC_A0, GX_CC_A1, GX_CC_A2, GX_CC_APREV};
                        result[i]->tev.c_in[j].arg =
                            in[result[i]->tev.c_in[j].exp->tev.a_dst];
                    }
                }
            }
        }
        if (result[i]->tev.a_dst != HSD_TE_UNDEF) {
            resource->reg[result[i]->tev.a_dst + 4].alpha = 1;

            for (j = 0; j < 4; j++) {
                if (HSD_TExpGetType(result[i]->tev.a_in[j].exp) == HSD_TE_TEV) {
                    static int in[4] = {GX_CA_A0, GX_CA_A1, GX_CA_A2, GX_CA_APREV};
                    result[i]->tev.a_in[j].arg =
                        in[result[i]->tev.a_in[j].exp->tev.a_dst];
                }
            }
        }
    }
}
