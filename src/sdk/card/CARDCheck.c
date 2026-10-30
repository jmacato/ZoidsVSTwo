#include "types.h"
#include "card.h"
#include "card_internal.h"

extern s32 CARDCheckExAsync(s32 chan, s32 *xferBytes, CARDCallback callback);

s32 CARDCheck(s32 chan)
{
    s32 result;
    s32 xferBytes;

    result = CARDCheckExAsync(chan, &xferBytes, __CARDSyncCallback);
    if (result >= 0) {
        if (&xferBytes == 0) {
            return result;
        }
        return __CARDSync(chan);
    }
    return result;
}
