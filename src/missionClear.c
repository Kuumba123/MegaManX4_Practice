#include <common.h>
#include "practice.h"

void MissionCleared(Game *gameP)
{
    gameP->startMode = 10;

    if (gameP->mode2 == 0)
    {
        gameP->mode2 = 1;
        FadeIn(9);
        return;
    }
    else if (gameP->mode2 == 1)
    {
        if (fadeDirection != 0)
        {
            return;
        }
        gameP->mode2 = 3;
    }
    else
    {
        while (true)
        {
            int time = practice.timer;
            int totalSeconds = time / 60;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            int frames = time % 60;

            DrawDebugText(6, 12, 0, "Your Clear Time was - %2d:%2d:%2d", minutes, seconds, frames);
            DrawDebugText(8, 14, 0, "Press Any Button to\nReturn to Stage Select");

            if (buttonsPressed != 0)
            {
                gameP->mode = 3;
                gameP->mode2 = 0;
                gameP->mode3 = 0;
                gameP->mode4 = 0;
                gameP->clearedStages = 0;
                return;
            }
            ThreadSleep(1);
        }
    }
}