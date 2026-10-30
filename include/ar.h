/* Types follow https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/ar.h.
 */
#ifndef ZOIDS_AR_H
#define ZOIDS_AR_H
#include "types.h"
typedef void (*ARCallback)(void);
typedef void (*ARQCallback)(u32 request);
typedef struct ARQRequest {
    struct ARQRequest *next;
    u32 owner;
    u32 type;
    u32 priority;
    u32 source;
    u32 dest;
    u32 length;
    ARQCallback callback;
} ARQRequest;
#define ARQ_PRIORITY_LOW 0
#define ARQ_PRIORITY_HIGH 1
#define ARQ_TYPE_MRAM_TO_ARAM 0
#define ARQ_TYPE_ARAM_TO_MRAM 1
extern ARCallback ARRegisterDMACallback(ARCallback callback);
extern void ARStartDMA(u32 type, u32 main_memory, u32 aram, u32 length);
extern u32 ARAlloc(u32 length);
extern u32 ARInit(u32 *block_lengths, u32 entries);
extern u32 ARGetBaseAddress(void);
extern u32 ARGetSize(void);
extern void ARQInit(void);
extern void ARQPostRequest(ARQRequest *request, u32 owner, u32 type, u32 priority,
                           u32 source, u32 destination, u32 length,
                           ARQCallback callback);
#endif
