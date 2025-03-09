#include <common.h>
#include "practice.h"

#define PAGE_TOTAL 3
#define Cursor gameP->mode3

static int page;

extern uint8_t playerFileTable[4];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);
void SaveRestore();
void LoadPlayerFile();

void CustomRoute(Game *gameP)
{
    page = 0;
    FadeIn(10);
    while (fadeDirection != 0)
    {
        ThreadSleep(1);
    }

    while (true)
    {
        if ((buttonsPressed & PAD_START) != 0)
        {
            for (size_t i = 0; i < 8; i++)
            {
                if ((gameP->collectables & (1 << i)) != 0)
                {
                    gameP->maxHP += 2;
                }
            }
            break;
        }

        bool toggle = (buttonsPressed & CONFIRM) != 0;

        switch (page)
        {
        case 0: // WEAPON PAGE
            if (toggle)
            {
                gameP->clearedStages ^= (1 << Cursor);
            }
            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (gameP->clearedStages & (1 << i)) != 0);
            }

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 7)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 7;
                }
            }
            DrawDebugText(12, 3, 2, "WEAPON PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "Lightning Web\nFrost Tower\nSoul Body\nRising Fire\nGround Hunter\nAiming Laser\nDouble Cyclone\nTwin Slasher");
            break;

        case 1: // ARMOR PAGE
            /*
             * HELMET,ARM,BODY,LEG
             */

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 5)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 5;
                }
            }

            if (Cursor < 4)
            {
                if (toggle)
                {
                    gameP->armorParts ^= (1 << Cursor);
                }
            }
            else if (Cursor == 4)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0 && gameP->busterType != 2)
                {
                    gameP->busterType += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0 && gameP->busterType != 0)
                {
                    gameP->busterType -= 1;
                }
                gameP->busterType &= 3;
            }
            else
            {
                if (toggle)
                {
                    gameP->cheatCodeFlag ^= 1;
                }
            }

            for (size_t i = 0; i < 4; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (gameP->armorParts & (1 << i)) != 0);
            }
            DrawDebugText(19, 9, 0, "%d", gameP->busterType);
            DrawDebugText(19, 10, 0, "%d", gameP->cheatCodeFlag);

            DrawDebugText(12, 3, 2, "ARMOR PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "Helmet\nBody\nArm\nLeg\nBuster Type\nCheat Enabled");
            break;

        case 2: // TANK PAGE
            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 7)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 7;
                }
            }

            if (Cursor == 0)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->mode4 += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->mode4 -= 1;
                }
                gameP->mode4 &= 7;

                if (toggle)
                {
                    gameP->collectables ^= (1 << gameP->mode4);
                }
            }
            else if (Cursor < 5)
            {
                if (toggle)
                {
                    gameP->collectables ^= (0x1000 << (Cursor - 1));
                }
            }
            else
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] -= 1;
                }
            }

            DrawDebugText(19, 5, 0, "%d", (gameP->collectables & (1 << gameP->mode4)) != 0);

            for (size_t i = 0; i < 4; i++)
            {
                DrawDebugText(19, 6 + i, 0, "%d", (gameP->collectables & (0x1000 << i)) != 0);
                if (i != 3)
                {
                    DrawDebugText(19, 10 + i, 0, "%d", gameP->tanksAmmo[i]);
                }
            }

            DrawDebugText(12, 3, 2, "TANK PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "HEART-%d:\nSUB-TANK 1\nSUB-TANK 2\nWEAPON-TANK\nEX-TANK\nTANK AMMO 1\nTANK AMMO 2\nTANK AMMO 3", gameP->mode4 + 1);
            break;

        default:
            break;
        }
        int8_t past = page;

        if ((buttonsPressed & PAD_R1) != 0)
        {
            if (page != (PAGE_TOTAL - 1))
            {
                page += 1;
            }
            else
            {
                page = 0;
            }
        }
        else if ((buttonsPressed & PAD_L1) != 0)
        {
            if (page != 0)
            {
                page -= 1;
            }
            else
            {
                page = (PAGE_TOTAL - 1);
            }
        }

        if (past != page)
        {
            Cursor = 0;
            gameP->mode4 = 0;
        }

        DrawDebugText(28, 3, 2, "%d/%d", page + 1, PAGE_TOTAL);
        DrawDebugText(9, 24, 1, "Press Start to\nStart the Stage");
        ThreadSleep(1);
    }
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    FadeOut(10);
    while (fadeDirection != 0)
    {
        ThreadSleep(1);
    }
    if (practice.playerFile != playerFileTable[game.player * 2 + game.cheatCodeFlag] && game.player == 0)
    {
        LoadPlayerFile();
    }
    
    SaveRestore();
}

#undef PAGE_TOTAL
#undef Cursor