#include "types.h"

s32 StrCmp(char *s1, char *s2)
{
    char c1;
    char c2;
    while (1) {
        c1 = *s1;
        c2 = *s2;
        if (c1 != c2)
            return 0;
        if (c1 == 0)
            break;
        s1++;
        s2++;
    }
    return 1;
}
