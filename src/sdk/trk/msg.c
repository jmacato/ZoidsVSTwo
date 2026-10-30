#include "types.h"
#include "metrotrk.h"

s32 TRKMessageSend(TRKBuffer *message)
{
    return TRK_WriteUARTN(message->data, message->length);
}
