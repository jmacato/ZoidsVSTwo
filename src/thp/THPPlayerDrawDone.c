#include "thp_player.h"

static BOOL Initialized;

extern OSMessage PopUsedTextureSet(void);
extern void PushFreeTextureSet(void *tex);

void THPPlayerDrawDone()
{
    GXDrawDone();
    if (Initialized) {
        while (TRUE) {
            void *tex = PopUsedTextureSet();
            if (tex == (void *)0)
                break;
            PushFreeTextureSet(tex);
        }
    }
}
