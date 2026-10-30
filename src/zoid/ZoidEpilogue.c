#include "model_archive.h"

extern ModelArchive *steFile;
extern struct HSD_MObj *steMobj;
extern ModelArchive *sdTest;

void ZoidEpilogue(void)
{
    if (steFile != 0) {
        PurgeArc2(steFile);
        steFile = 0;
    }
    steMobj = 0;
    PurgeArc2(sdTest);
    sdTest = 0;
}
