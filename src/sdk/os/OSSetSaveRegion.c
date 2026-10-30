extern void *SaveStart;
extern void *SaveEnd;

void OSSetSaveRegion(void *start, void *end)
{
    SaveStart = start;
    SaveEnd = end;
}
