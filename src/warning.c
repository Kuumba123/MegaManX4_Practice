#include <common.h>
#include <object.h>

#define arcEffectP (*(Object **)((int)objP + 0x14))

void WarningObject(Object *objP)
{
    if (arcEffectP->flags == 0)
    {
        objP->act = 2;
    }
}