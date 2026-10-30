#include "model_archive.h"
#include "mobj.h"
extern ModelArchive *Sky;
extern GXColor SkyAmbient;
extern HSD_MObj *searchMObj(HSD_JObj *);
void getSkyAmbient(void)
{
    HSD_JObj *joint;
    HSD_MObj *material;
    HSD_Material *mat;
    if (Sky != (void *)0) {
        joint = Sky->models[1];
        if (joint != (void *)0) {
            material = searchMObj(joint);
            if (material != (void *)0) {
                mat = material->mat;
                if (mat != (void *)0)
                    SkyAmbient = mat->ambient;
            }
        }
    }
}
