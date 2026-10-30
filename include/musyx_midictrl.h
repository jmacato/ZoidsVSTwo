#ifndef ZOIDS_MUSYX_MIDICTRL_H
#define ZOIDS_MUSYX_MIDICTRL_H

#include "types.h"

/* Reference: https://github.com/AxioDL/musyx/blob/main/include/musyx/musyx.h (CC0-1.0). */
#define SND_MIDICTRL_MODULATION 0x01
#define SND_MIDICTRL_VOLUME 0x07
#define SND_MIDICTRL_PANNING 0x0A
#define SND_MIDICTRL_PEDAL 0x40
#define SND_MIDICTRL_PORTAMENTO 0x41
#define SND_MIDICTRL_REVERB 0x5B
#define SND_MIDICTRL_CHORUS 0x5D

#define SND_MIDICTRL_PITCHBEND 0x80
#define SND_MIDICTRL_SPANNING 0x83
#define SND_MIDICTRL_DOPPLER 0x84

extern void inpSetMidiCtrl(u8 ctrl, u8 channel, u8 set, u8 value);
extern void inpSetMidiCtrl14(u8 ctrl, u8 channel, u8 set, u16 value);
extern u16 inpGetMidiCtrl(u8 ctrl, u8 channel, u8 set);
extern void inpSetMidiLastNote(u8 midi, u8 midiSet, u8 key);
extern u8 inpGetMidiLastNote(u8 midi, u8 midiSet);

#endif
