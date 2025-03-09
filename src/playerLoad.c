#include <common.h>
#include "practice.h"

#if BUILD == 561
uint8_t playerFileTable[4] = {0x4B, 0x4E, 0x4D, 0x4D};
#else
uint8_t playerFileTable[4] = {0x4B, 0x4F, 0x4D, 0x4D};
#endif

extern void * levelBufferP;
extern void * levelVabP;
extern void * lastVabP;

void LoadScreenInfoFade();
void LoadPlayerSEP();

void LoadPlayerFile()
{
    practice.playerFile = playerFileTable[game.player * 2 + game.cheatCodeFlag];
    ArcSeek(practice.playerFile, 0, 0);
    DrawLoad(false, false);
    LoadScreenInfoFade();
    levelBufferP = freeArcP;
    levelVabP = lastVabP;
    LoadPlayerSEP();
}