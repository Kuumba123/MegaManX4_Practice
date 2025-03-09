#include <common.h>
#include <object.h>

#if BUILD == 561
#define BOSSFILE *(uint8_t *)0x80171ea8
#define ST0C_1_ARC_ID 0x6D
#define SCREENBACKUP *(uint32_t *)0x801441b4
#else
#define BOSSFILE *(uint8_t *)0x80171f60
#define ST0C_1_ARC_ID 0x6C
#define SCREENBACKUP *(uint32_t *)0x80144294
#endif

extern int8_t checkPointNew;
extern void * endFreeArcP;

void DeleteEffectObject(int8_t id,uint8_t var);
void LoadScreens();

void DetermineClear(Game *gameP)
{
    if (gameP->clear == 0)
    {
        gameP->specialStart = 0;
        bool reset = false;

        if (gameP->stageId == 0xC && gameP->mid == 0 && gameP->point > 1)
        {
            reset = gameP->point < 10;
        }

        /* Teleporter Room in Refights */
        if (reset)
        {
            gameP->point = 1;
        }

        gameP->mode = 5;
        
        EndSong(); // PS2 POPS Fix

        if (gameP->stageId == 0xC && gameP->mid != 0 && BOSSFILE != 0)
        {
            BOSSFILE = 0;
            ArcSeek(ST0C_1_ARC_ID, 3, 0);
            DrawLoad(0, 0);
            freeArcP = endFreeArcP;
        }
        else
        {
            LoadScreens();
        }
    }
    else
    {
        if (gameP->clear < 0)
        {
            DeleteEffectObject(2, 0xE);
            for (size_t i = 0; i < 16; i++)
            {
                if ((uint8_t)gameP->clear != 0xC1)
                {
                    gameP->ammoTemp[i] = mega.ammo[i];
                }
                else
                {
                    gameP->ammoTemp[i] = 48;
                }
            }
            
            gameP->specialStart = 0xFF;
            gameP->currentMaxHP = mega.hp;
            gameP->weaponTemp = mega.weapon;

            if ((uint8_t)gameP->clear == 0xC1)
            {
                if (gameP->stageId == 0xC && gameP->mid != 0)
                {
                    BOSSFILE = 1;
                }
                freeArcP = endFreeArcP;
                gameP->point = checkPointNew;
                gameP->weaponTemp = 0;
                gameP->currentMaxHP = gameP->maxHP;
            }
            if (gameP->stageId != 0xC)
            {
                gameP->songOff = true;
            }
            if (gameP->stageId == 0xC && gameP->mid == 0 && gameP->point == 10)
            {
                gameP->songOff = true;
            }

            gameP->mode = 5;
        }
        else // Actual Real Clear
        {
            gameP->specialStart = 0;
            if ((gameP->stageId < 9 || gameP->stageId == 0xB || gameP->stageId == 0xC) && gameP->mid == 0)
            {
                gameP->mid = 1;
                gameP->mode = 4;
            }
            else
            {
                gameP->mode = 9;
            }
        }
    }
    gameP->mode2 = 0;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    gameP->clear = 0;
}

#undef BOSSFILE
#undef ST0C_1_ARC_ID