#include "particle.h"

extern void __assert(const char *file, u32 line, const char *condition);

void psInterpretParticles(u32 mask)
{
    s32 has_next, same_next, different_result, has_previous;
    HSD_Particle *next;
    HSD_Particle *particle;
    HSD_Particle *previous;
    s32 link;

    for (link = 0; link < 16; link++, mask >>= 1) {
        if (!(mask & 0x10000)) {
            previous = 0;
            particle = _psListGetFirst(link);
            while (particle != 0) {
                HSD_Particle *result;
                next = particle->next;
                result = psInterpretParticle0(particle, previous);
                has_next = next != 0;
                if (has_next) {
                    same_next = particle->next == next;
                    if (same_next) {
                        previous = particle;
                    } else {
                        different_result = result != next;
                        if (different_result) {
                            previous = result;
                            while (previous->next != next) {
                                has_previous = previous != 0;
                                if (!has_previous)
                                    __assert("psinterpret.c", 1451, "lastPP");
                                previous = previous->next;
                            }
                        }
                    }
                }
                particle = next;
            }
        }
    }
}
