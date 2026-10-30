/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/rumble.c. */
#include "controller.h"
extern int OSDisableInterrupts(void);
extern int OSRestoreInterrupts(int enabled);
extern void PADControlMotor(int channel, u32 command);
#define PAD_MOTOR_STOP 0
#define PAD_MOTOR_RUMBLE 1
#define PAD_MOTOR_STOP_HARD 2
static HSD_RumbleData HSD_PadRumbleStatus[4];
void HSD_PadRumbleOn(u8 no)
{
    int interrupts = OSDisableInterrupts();
    HSD_RumbleData *status = &HSD_PadRumbleStatus[no];

    status->direct_status = 2;
    OSRestoreInterrupts(interrupts);
}

void HSD_PadRumbleOffN(u8 no)
{
    int interrupts = OSDisableInterrupts();
    HSD_RumbleData *status = &HSD_PadRumbleStatus[no];

    status->direct_status = 1;
    OSRestoreInterrupts(interrupts);
}

static inline void HSD_PadRumbleFree(HSD_RumbleData *status,
                                     HSD_PadRumbleListData *script)
{
    RumbleInfo *pool = &HSD_PadLibData.rumble_info;
    HSD_PadRumbleListData **link = &status->listdatap;

    while ((*link) != script) {
        link = &(*link)->next;
    }
    *link = script->next;
    status->nb_list--;
    script->next = pool->listdatap;
    pool->listdatap = script;
}

static int HSD_PadRumbleInterpret1(HSD_PadRumbleListData *script, u8 *motor_status)
{
    if (script->pause == 1) {
        return 0;
    }
    while (script->wait == 0) {
        switch (script->listp->command.op) {
        case 0:
            if (script->frame == -2) {
                return 1;
            }
            script->listp = script->headp;
            break;
        case 1:
            script->status = 2;
            script->wait = script->listp->command.frame;
            script->listp++;
            break;
        case 2:
            script->status = 1;
            script->wait = script->listp->command.frame;
            script->listp++;
            break;
        case 3:
            script->status = 0;
            script->wait = script->listp->command.frame;
            script->listp++;
            break;
        case 4:
            script->loop_count = script->listp->command.frame;
            script->listp++;
            script->stack = script->listp;
            break;
        case 5:
            if (--script->loop_count != 0) {
                script->listp = script->stack;
            } else {
                script->listp++;
            }
            break;
        }
    }
    *motor_status = script->status;
    script->wait--;
    if (script->frame != -1 && script->frame != -2) {
        if (--script->frame == 0) {
            return 1;
        }
    }
    return 0;
}

void HSD_PadRumbleInterpret(void)
{
    struct RumbleInfo *pool = &HSD_PadLibData.rumble_info;
    HSD_RumbleData *status = HSD_PadRumbleStatus;
    HSD_PadRumbleListData *script;
    HSD_PadRumbleListData *next;

    int i;
    for (i = 0; i < 4; i++, status++) {
        {
            status->status = status->direct_status;
            script = status->listdatap;
            while (script != 0) {
                next = script->next;

                if (HSD_PadRumbleInterpret1(script, &status->status) != 0) {
                    HSD_PadRumbleFree(status, script);
                }
                script = next;
            }
        }
        if (status->status != status->last_status) {
            switch (status->status) {
            case 0:
                PADControlMotor(i, PAD_MOTOR_STOP_HARD);
                break;
            case 1:
                PADControlMotor(i, PAD_MOTOR_STOP);
                break;
            case 2:
                PADControlMotor(i, PAD_MOTOR_RUMBLE);
                break;
            }
            status->last_status = status->status;
        }
    }
}

void HSD_PadRumbleInit(u16 count, void *records)
{
    struct RumbleInfo *pool = &HSD_PadLibData.rumble_info;
    int i;

    pool->max_list = count;
    pool->listdatap = records;
    if (count != 0) {
        for (i = 0; i < count - 1; i++) {
            pool->listdatap[i].next = &pool->listdatap[i + 1];
        }
        pool->listdatap[i].next = 0;
    }
    {
        static HSD_RumbleData default_status_data = {0};
        for (i = 0; i < 4; i++) {
            HSD_PadRumbleStatus[i] = default_status_data;
        }
    }
}
