#include <common.h>
#include <object.h>


static struct Title
{
    uint8_t mode;
    uint8_t mode2;
    uint8_t mode3;
    uint8_t mode4;

    int16_t timer;
};

static struct TitleText
{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

extern struct TitleText titleTextData[7];

extern struct Title title;

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static void MiscObj_13_1(Object *objP)

{
    if (objP->stageVar == 0xf)
    {
        /* Hightlighted Cursor */
        objP->priority = 2;
        *(int16_t *)((int)&objP->y + 2) = (title.mode3 >> 1) * 16 + 128 + 16;
        if (title.mode3 == 1)
        {
            objP->animeInfo.sprtFrame = 0x1d;
        }
        else if ((int8_t)title.mode3 < 2 && title.mode3 == 0)
        {
            (objP->animeInfo).sprtFrame = 2;
        }
        else
        {
            (objP->animeInfo).sprtFrame = 0x1e;
        }
    }
    if (objP->stageVar >= 4 && objP->stageVar <= 6)
    {
        objP->display = false;
        if ((frameCount & 0x10) == 0)
        {
            return;
        }
    }
    else
    {
        objP->display = true;
    }
    DisplayObject(objP);
}