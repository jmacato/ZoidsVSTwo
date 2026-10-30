#include "types.h"
#include "card.h"
#include "card_internal.h"

extern s32 OSGetFontEncode(void);
extern s32 __CARDFormatRegionAsync(s32 chan, s32 encode, CARDCallback callback);

s32 CARDFormat(s32 chan)
{
    s32 result;

    result = __CARDFormatRegionAsync(chan, OSGetFontEncode(), __CARDSyncCallback);
    if (result < 0) {
        return result;
    }
    return __CARDSync(chan);
}
