#ifndef ZOIDS_RUNTIME_DECIMAL_H
#define ZOIDS_RUNTIME_DECIMAL_H
#define SIGDIGLEN 36
typedef struct decimal {
    char sign;
    short exp;
    struct {
        unsigned char length;
        unsigned char text[SIGDIGLEN];
    } sig;
} decimal;
typedef struct decform {
    char style;
    short digits;
} decform;
void __num2dec(const decform *format, double value, decimal *result);
#endif
