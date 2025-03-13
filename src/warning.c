#include <common.h>
#include <object.h>
#include <layer.h>

#define arcEffectP (*(Object **)((int)objP + 0x14))

void WarningObject(Object *objP)
{
    if (arcEffectP->flags == 0)
    {
        if (game.stageId == 0x7 && bgLayers[0].x != 0x8C0)
        {
            return;
        }
        objP->act = 2;
    }
}