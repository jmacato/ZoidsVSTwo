/* ABI from
 * https://github.com/doldecomp/melee/blob/master/src/MSL/stdarg.h. */
#ifndef ZOIDS_MWCC_STDARG_H
#define ZOIDS_MWCC_STDARG_H

#ifdef __clang__
typedef __builtin_va_list va_list;
#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#else

typedef struct __va_list_t {
    char gpr;
    char fpr;
    char reserved[2];
    char *input_arg_area;
    char *reg_save_area;
} __va_list[1];
typedef __va_list va_list;

extern void __builtin_va_info(void *);

void *__va_arg(va_list v_list, unsigned char type);

#define va_start(ap, fmt) ((void)fmt, __builtin_va_info(&ap))
#define va_arg(ap, t) (*((t *)__va_arg(ap, _var_arg_typeof(t))))
#define va_end(ap) (void)0
#endif

#endif
