#include <common.h>
#include <gpu.h>
#include "practice.h"

#if BUILD == 561
#define MENU_TXT "X = SPAWN\nO = RESTART\n^ = EXIT"
#define RESET PAD_CIRCLE
#define BOSSFILE *(uint8_t *)0x80171ea8
#define MAIN_THREAD 0x8001fb50
#else
#define MENU_TXT "O  = SPAWN\n[] = RESTART\nX  = EXIT"
#define RESET PAD_SQUARE
#define BOSSFILE *(uint8_t *)0x80171f60
#define MAIN_THREAD 0x8001fbac
#endif

static struct Restore
{
    uint16_t collectables;
    int8_t maxHP;
    uint8_t busterType;
    uint8_t armorParts;
    int8_t stageId;
    int8_t mid;
    int8_t tanksAmmo[3];
};
static struct Restore restore;

int8_t checkPointNew = 0xFF;

int8_t maxCheckPoint[] =
    {
        2, 2,  // ST00
        4, 5,  // ST01
        4, 3,  // ST02
        3, 3,  // ST03
        1, 3,  // ST04
        0, 1,  // ST05
        5, 1,  // ST06
        2, 3,  // ST07
        2, 3,  // ST08
        0, 0,  // ST09
        2, 0,  // ST0A
        1, 2,  // ST0B
        10, 0  // ST0C
};

void SetupSpawn();

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SaveRestore()
{
    restore.collectables = game.collectables;
    restore.maxHP = game.maxHP;
    restore.busterType = game.busterType;
    restore.armorParts = game.armorParts;
    restore.stageId = game.stageId;
    restore.mid = game.mid;
    restore.tanksAmmo[0] = game.tanksAmmo[0];
    restore.tanksAmmo[1] = game.tanksAmmo[1];
    restore.tanksAmmo[2] = game.tanksAmmo[2];    
}
void LoadRestore()
{
    game.collectables = restore.collectables;
    game.maxHP = restore.maxHP;
    game.busterType = restore.busterType;
    game.armorParts = restore.armorParts;
    game.stageId = restore.stageId;
    game.mid = restore.mid;
    game.tanksAmmo[0] = restore.tanksAmmo[0];
    game.tanksAmmo[1] = restore.tanksAmmo[1];
    game.tanksAmmo[2] = restore.tanksAmmo[2];
}

void CheckPointCheck(Game *gameP)
{
    if (gameP->clear == 0 && mega.hp != 0)
    {
        if ((buttonsHeld & (PAD_RIGHT + PAD_SELECT)) == (PAD_RIGHT + PAD_SELECT))
        {
            checkPointNew = gameP->point + 1;
            gameP->clear = 0xC1;
        }
        else if ((buttonsHeld & (PAD_LEFT + PAD_SELECT)) == (PAD_LEFT + PAD_SELECT))
        {
            checkPointNew = gameP->point - 1;
            gameP->clear = 0xC1;
        }
        else if ((buttonsHeld & (PAD_L1 + PAD_R1 + PAD_TRIANGLE + PAD_SELECT)) == (PAD_L1 + PAD_R1 + PAD_TRIANGLE + PAD_SELECT))
        {
            // Checkpoint Menu
            int mouse = 0;
            int8_t max = maxCheckPoint[game.stageId * 2 + game.mid];

            while (true) // Loop
            {
                if ((buttonsPressed & CANCEL) != 0)
                {
                    break;
                }

                // Check Inputs
                if ((buttonsPressed & CONFIRM) != 0)
                {
                    checkPointNew = mouse;
                    game.clear = 0xC1;
                    break;
                }
                else if ((buttonsPressed & RESET) != 0)
                {
                    game.mode = 4;
                    game.mode2 = 0;
                    game.mode3 = 0;
                    game.mode4 = 0;
                    game.point = 0;
                    game.mid = 0;
                    BOSSFILE = 0;
                    TurnOffSound(0xFF,0);
                    EndSong();
                    LoadRestore();
                    NewThread2(MAIN_THREAD);
                }

                if ((buttonsPressed & PAD_DOWN) != 0)
                {
                    mouse++;
                }
                else if ((buttonsPressed & PAD_UP) != 0)
                {
                    mouse--;
                }

                if (mouse < 0)
                {
                    mouse = max;
                }
                else if (mouse > max)
                {
                    mouse = 0;
                }

                DrawDebugText(3, 5 + mouse, 1, ">");
                for (size_t i = 0; i < (max + 1); i++)
                {
                    DrawDebugText(4, 5 + i, 0, "POINT %d", i);
                }
                DrawDebugText(18, 22, 1, MENU_TXT);
                DrawDebugText(12, 3, 2, "CHECKPOINT PAGE");
                ThreadSleep(1);
            }
        }

        if (checkPointNew > maxCheckPoint[gameP->stageId * 2 + gameP->mid])
        {
            checkPointNew = 0;
        }
        else if (checkPointNew < 0)
        {
            checkPointNew = maxCheckPoint[gameP->stageId * 2 + gameP->mid];
        }
    }
}
void ValidCheckPoint()
{
    SetupSpawn();
    if (game.point > 9)
    {
        game.point = 9;
    }
}
#undef MENU_TXT
#undef RESET
#undef RELOAD
#undef MAIN_THREAD