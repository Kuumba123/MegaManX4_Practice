#include <common.h>

/*
/*  Japanese Version spefic File
*/

extern uint8_t songState;

void DetermineBossSong();

uint32_t NewPlayBossSong()
{
    bool bVar1;

    if (((songState == 0) && (game.startingSong != 0)) &&
        (bVar1 = game.startingSong == 1, game.startingSong = game.startingSong - 1, bVar1))
    {
        game.startingSong = 0;
        DetermineBossSong();
    }
    return (uint32_t)game.startingSong;
}