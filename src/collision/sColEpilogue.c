#include "ground_collision.h"

extern void free(void *memory);

void sColEpilogue(void)
{
    if (grdcCol != (void *)0) {
        free(grdcCol);
        grdcCol = 0;
    }
}
