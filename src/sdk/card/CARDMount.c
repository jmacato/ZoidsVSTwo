#include "card.h"
#include "card_internal.h"

s32 CARDMount(s32 chan, void *workArea, CARDCallback detachCallback)
{
    s32 result;

    result = CARDMountAsync(chan, workArea, detachCallback, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }
    result = __CARDSync(chan);
    return result;
}
