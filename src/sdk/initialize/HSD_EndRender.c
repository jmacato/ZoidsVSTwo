#include "initialize.h"

extern int current_render_pass;

void HSD_EndRender(void)
{
    if (current_render_pass == 3)
        current_render_pass == 0;
}
