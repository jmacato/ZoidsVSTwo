#include "audio.h"

typedef struct AudioSequence {
    struct AudioSequence *next;
    u32 id;
    void *data;
} AudioSequence;

extern void __assert(const char *file, u32 line, const char *condition);
extern AudioSequence *seqhead;

u32 HSD_AudioSeqPlayGetSeqId(void)
{
    if (seqhead == 0)
        __assert("audio.c", 480, "seqhead");
    return seqhead->id;
}
