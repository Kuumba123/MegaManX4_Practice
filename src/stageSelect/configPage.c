#include <common.h>
#include <gpu.h>
#include "../practice.h"

#define Cursor gameP->mode3
#if BUILD == 561
#define VOLUME_ADDR *(uint32_t*) 0x80016898
#else
#define VOLUME_ADDR *(uint32_t*) 0x800168cc
#endif

#define RECT_X 16
#define RECT_Y 30
#define RECT_W 287
#define RECT_H 172

static TILE rect[2];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static char* CategoryStrings[2][6] = {
    {"100%", "Any%", "Any% (New)", "Low%", "UA Any%", "Custom"},
    {"100%", "100% Stringray", "Custom", "", "", ""}
};


void ConfigPage(Game *gameP)
{
    setTile(&rect[buffer]);
    setRGB0(&rect[buffer],0,0,0);
    setXY0(&rect[buffer], RECT_X, RECT_Y);
    setWH(&rect[buffer], RECT_W, RECT_H);
    addPrim(&drawP->ot[0],&rect[buffer]);

    if ((buttonsPressed & PAD_R1) != 0)
    {
        gameP->mode2 = 4;
        PlaySound(5, 0, 0);
        return;
    }

    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        if (Cursor == 2)
        {
            Cursor = 0;
        }
        else
        {
            Cursor += 1;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (Cursor == 0)
        {
            Cursor = 2;
        }
        else
        {
            Cursor -= 1;
        }
    }

    bool toggle = (buttonsPressed & (PAD_LEFT + PAD_RIGHT + CONFIRM)) != 0;

    int max = 2;

    if (gameP->player == 0)
    {
        max = 5;
    }
    

    if (Cursor == 0) //Category
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            if (practice.category[game.player] == max)
            {
                practice.category[game.player] = 0;
            }
            else
            {
                practice.category[game.player] += 1;
            }
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            if (practice.category[game.player] == 0)
            {
                practice.category[game.player] = max;
            }
            else
            {
                practice.category[game.player] -= 1;
            }
        }
        
    }
    else if (Cursor == 1 && toggle)
    {
        practice.keepRng ^= 1;
    }
    else if (Cursor == 2 && toggle)
    {
        practice.cancelXA ^= 1;
    }

    if (practice.cancelXA == false)
    {
        VOLUME_ADDR = 0x324400FF;
    }
    else
    {
        VOLUME_ADDR = 0x34040000;
    }

    DrawDebugText(3,6 + Cursor,1,">");
    DrawDebugText(4,6,0,"Category\nKeep RNG\nCancel XA");
    DrawDebugText(21,6,0,"%s\n%d\n%d",CategoryStrings[game.player][practice.category[game.player]],practice.keepRng,practice.cancelXA);
    DrawDebugText(14, 4, 2, "CONFIG PAGE");
}

#undef Cursor
#undef RECT_X
#undef RECT_Y
#undef RECT_W
#undef RECT_H
#undef VOLUME_ADDR