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
        18, 0  // ST0C
};

static DR_TPAGE livesTpage[2];
static SPRT_16 livesSprt[2][2];

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
            ThreadSleep(1);

            // Checkpoint Menu
            int mouse = 0;
            int8_t max = maxCheckPoint[gameP->stageId * 2 + gameP->mid];

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
                    gameP->clear = 0xC1;
                    break;
                }
                else if ((buttonsPressed & RESET) != 0)
                {
                    gameP->mode = 4;
                    gameP->mode2 = 0;
                    gameP->mode3 = 0;
                    gameP->mode4 = 0;
                    gameP->point = 0;
                    gameP->mid = 0;
                    BOSSFILE = 0;

                    for (size_t i = 0; i < 10; i++)
                    {
                        gameP->refights[i] = 0;
                    }

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
void ShowPoint(int lives)
{
    /*Orignal Position is X: 31 , Y: 80*/
    for (size_t i = 0; i < 2; i++)
    {
        if (lives < 10 && i != 0)
        {
            setlen(&livesSprt[buffer][i],0);
        }
        else
        {
            setlen(&livesSprt[buffer][i],3);
        }
        
        setcode(&livesSprt[buffer][i],0x7D);

        livesSprt[buffer][i].y0 = 80;
        
        if (i == 0)
        {
            setUV0(&livesSprt[buffer][i],(lives % 10) * 16, 240);
            if (lives > 9)
            {
                livesSprt[buffer][i].x0 = 31 + 1;
            }
            else
            {
                livesSprt[buffer][i].x0 = 31;
            }
        }
        else
        {
            livesSprt[buffer][i].x0 = 29;
            setUV0(&livesSprt[buffer][i],1 * 16, 240);
        }
        setClut(&livesSprt[buffer][i],16,481);
    }
    setlen(&livesTpage[buffer],1);
    catPrim(&livesTpage[buffer],&livesSprt[buffer][0]);
    catPrim(&livesSprt[buffer][0],&livesSprt[buffer][1]);
    livesTpage[buffer].code[0] = 0xE1000005;
    addPrims(&drawP->ot[3],&livesTpage[buffer],&livesSprt[buffer][1]);
}
#undef MENU_TXT
#undef RESET
#undef RELOAD
#undef MAIN_THREAD