#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "../practice.h"

extern uint8_t playerFileTable[4];

static uint8_t Cursor;
static uint8_t AreaMode;
static uint8_t CategoryOptionData;
static bool isRevist = false;
static uint8_t stageIdTable[8] = {1, 6, 7, 4, 5, 3, 8, 2};

/*X 100%*/
static uint8_t hundoXClearedTable[8] = {8, 0x49, 0xDB, 0, 0x4B, 0xDF, 9, 0x5B};
static uint16_t hundoXCollectablesTable[8] = {0, 0x41, 0xE0DB, 0, 0xC043, 0xE0DF, 1, 0xE053};
static uint8_t hundoXArmorPartsTable[8] = {0, 0xC, 0xE, 0, 0xC, 0xE, 8, 0xC};

/*X Any%*/
static uint8_t anyXClearedTable[8] = {0, 0xC9, 0xDB, 0xC1, 0xCB, 0xDF, 1, 0x41};
static uint16_t anyXCollectablesTable[8] = {0, 0xC0, 0x60D0, 0xC0, 0x40C0, 0x60D0, 0, 0x40};
static uint8_t anyXArmorPartsTable[8] = {0, 6, 6, 4, 6, 6, 0, 4};

/*X Any% (New Route)*/
static uint8_t anyNewXCleartedTable[8] = {0, 0x49, 0xDB, 1, 0x4B, 0xDF, 9, 0x5B};
static uint16_t anyNewCollectablesTable[8] = {0, 0x40, 0x60D0, 0, 0x4040, 0x60D0, 0, 0x6050};
static uint8_t anyNewXArmorPartsTable[8] = {0, 6, 6, 0, 6, 6, 2, 6};

/*X Low%*/
static uint8_t lowXCleartedTable[8] = {8, 9, 0x9B, 0, 0xB, 0x9F, 0xBF, 0x1B};

/*Ultimate Armor Any%*/
static uint8_t uaXClearedTable[8] = {0x28, 0x29, 0xBB, 0x20, 0x2B, 0, 0xBF, 0x3B};
static uint16_t uaXCollectablesTable[8] = {0x1020, 0x1020, 0x30B0, 0x1020, 0x1020, 0, 0x30B0, 0x3030};

/*Zero 100%*/
static uint8_t hundoZeroCleartedTable[8] = {0x3C, 0xFD, 0x28, 0, 0x2C, 8, 0x3D, 0x7D};
static uint16_t hundoZeroCollectablesTable[8] = {0x303C, 0x30FD, 0x1028, 0, 0x102C, 8, 0x303D, 0x307D};

/*Zero 100% (Stingray 1st)*/
static uint8_t hundoZeroStingrayClearedTable[8] = {0xBE, 0xBC, 0x38, 0x10, 0, 0x18, 0xBF, 0x3C};
static uint16_t hundoZeroStingrayCollectablesTable[8] = {0xF0BE, 0x30BC, 0x3038, 0x2010, 0, 0x2018, 0xF0BF, 0x303C};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void StageSelectLoop(Game *gameP);

void CustomRoute(Game *gameP);

void SaveRestore();

static TILE rect[2];

void StageSelect(Game *gameP)
{
    if ((buttonsPressed & PAD_L1) != 0) // Goto Config Page
    {
        gameP->mode2 = 8;
        gameP->mode3 = 0;
        PlaySound(5, 0, 0);
        return;
    }
    else if ((buttonsPressed & (PAD_START + CONFIRM)) != 0)
    {
        /*
         *   0 = Sigma 1,2,3, & Intro
         *   1 = Start & Mid
         *   2 = Vist & ReVist
         */
        PlaySound(5, 1, 0);
        Cursor = 0;
        AreaMode = 0;
        CategoryOptionData = 0;
        isRevist = false;
        gameP->mode2 = 9;
        gameP->mode3 = 0;
        if (gameP->mode4 == 8)
        {
            gameP->mode2 = 9;
            return;
        }
        AreaMode = 1;
        gameP->stageId = stageIdTable[gameP->mode4];
        if (gameP->stageId == 4 && gameP->player == 0 && practice.category[0] == X_HUNDO)
        {
            CategoryOptionData = 5;
            AreaMode = 2;
        }
        
    }
    else
    {
        if ((frameCount & 0x10) != 0)
        {
            DrawDebugText(2, 11, 1, "(L1)\n\nC\no\nn\nf\ni\ng");
        }
        
        StageSelectLoop(gameP);
    }
}

static const char *stageNames[] = {"SIGMA 1", "SIGMA 2", "SIGMA 3", "Intro", "Colonel"};
static const char *halfText[] = {"Start", "Mid"};
static const char *vistText[] = {"Vist","Re-Vist"};

void StageSelectArea(Game *gameP)
{
    setTile(&rect[buffer]);
    setRGB0(&rect[buffer],0,0,0);
    setXY0(&rect[buffer], 188, 88);
    setWH(&rect[buffer], 80, 64);
    addPrim(&drawP->ot[0],&rect[buffer]);
    DrawDebugText(25, 13, 0, "Area");
    if (AreaMode == 0)
    {
        DrawDebugText(25, 15, 2, stageNames[Cursor]);
    }
    else if(AreaMode == 1)
    {
        DrawDebugText(25, 15, 2, halfText[Cursor]);
    }
    else
    {
        DrawDebugText(25, 15, 2, vistText[Cursor]);
    }
    

    int pastCursor = Cursor;

    if ((buttonsPressed & (PAD_LEFT + PAD_RIGHT)) != 0)
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            Cursor += 1;
            if (gameP->player != 0 && Cursor == 4)
            {
                Cursor = 0;
            }
        }
        else
        {
            Cursor -= 1;
        }
        if (AreaMode != 0)
        {
            Cursor &= 1;
        }
        else
        {
            if (Cursor == 0xFF)
            {
                Cursor = 3;
                if (gameP->player == 0)
                {
                    Cursor = 4;
                }
            }
            Cursor %= 5;
        }
    }
    if (pastCursor == Cursor)
    {
        if ((buttonsPressed & (PAD_START + CONFIRM)) == 0)
        {
            if ((buttonsPressed & CANCEL) != 0) // Triangle was pressed , go back to maverick select
            {
                gameP->mode2 = 4;
            }
        }
        else
        {
            // route checks and such here
            PlaySound(5, 1, 0);

            if (AreaMode == 0)
            {
                if (Cursor == 3)
                {
                    gameP->stageId = 0;
                    AreaMode = 1;
                    Cursor = 0;
                    return;
                }
                else if (Cursor == 4)
                {
                    gameP->stageId = 9;
                }
                else
                {
                    gameP->stageId = Cursor + 0xA;
                    if (Cursor != 0)
                    {
                        AreaMode = 1;
                        Cursor = 0;
                        return;
                    }
                }
            }
            else if (AreaMode == 2) // Vist & ReVist
            {
                uint8_t option = 0;
                isRevist = Cursor;
                if (isRevist == false)
                {
                    option = CategoryOptionData & 3;
                }
                else
                {
                    option = (CategoryOptionData >> 2) & 3;
                }
                if (option != 0)
                {
                    AreaMode = option;
                    Cursor = 0;
                    return;
                }
            }
            else if (AreaMode == 1) // Start & Mid
            {
                gameP->mid = Cursor;
                if ((CategoryOptionData & 16) != 0 && Cursor == 0)
                {
                    AreaMode = 3;
                    Cursor = 0;
                    return;
                }
            }
            if (gameP->player == 0)
            {
                gameP->sceneFlags = 0x31;
            }
            else
            {
                gameP->sceneFlags = 0x11;
            }
            gameP->mode2 = 5;
        }
    }
    else
    {
        PlaySound(5, 0, 0);
    }
}
void StageSelectDetermine(Game *gameP)
{
    if (fadeDirection != 0)
    {
        return;
    }
    DefaultLayers();
    ClearAll();

    gameP->mode = 4;
    gameP->mode2 = 0;
    gameP->mode3 = 0;
    gameP->mode4 = 0;

    practice.timer = 0;

    gameP->lives = 2;
    gameP->clearedStages = 0;
    gameP->maxHP = 32;
    gameP->armorParts = 0;
    gameP->collectables = 0;
    gameP->tanksAmmo[0] = 0;
    gameP->tanksAmmo[1] = 0;
    gameP->tanksAmmo[2] = 0;
    gameP->busterType = 0;
    gameP->cheatCodeFlag = (practice.category[0] == UA_PERCENT && gameP->player == 0);

    if ((practice.category[0] == X_CUSTOM && gameP->player == 0) || (practice.category[1] == ZERO_CUSTOM && gameP->player != 0))
    {
        CustomRoute(gameP);
        return;
    }

    if (gameP->stageId > 9) // Sigma Stages
    {
        gameP->clearedStages = 0xFF;
        
        if ((practice.category[0] == X_HUNDO && gameP->player == 0) || ((practice.category[1] == ZERO_HUNDO || practice.category[1] == ZERO_HUNDO_STRINGRAY) && gameP->player != 0))
        {
            gameP->collectables = 0xF0FF;
            if (gameP->player == 0)
            {
                gameP->armorParts = 0xF;
                gameP->busterType = 1;
            }
        }
        else if (gameP->player == 0)
        {
            if (practice.category[0] == X_ANY_PERCENT || practice.category[0] == X_ANY_PERCENT_NEW)
            {
                gameP->collectables = 0x70F0;
                gameP->armorParts = 6;
                gameP->busterType = 1;
            }
            else if (practice.category[0] == UA_PERCENT)
            {
                gameP->collectables = 0x30F0;
                gameP->armorParts = 0xF;
                gameP->busterType = 2;
            }
            
        }
    }
    else if (gameP->stageId == 9) // Colonel
    {
        if (practice.category[0] == X_HUNDO)
        {
            gameP->clearedStages = 0x4B;
            gameP->armorParts = 0xC;
            gameP->busterType = 1;
            gameP->collectables = 0xC043;
        }
        else if (practice.category[0] == X_ANY_PERCENT)
        {
            gameP->clearedStages = 0xC9;
            gameP->collectables = 0xC0;
            gameP->armorParts = 6;
            gameP->busterType = 1;
        }
        else if (practice.category[0] == X_ANY_PERCENT_NEW)
        {
            gameP->clearedStages = 0x4B;
            gameP->collectables = 0x4040;
            gameP->armorParts = 6;
            gameP->busterType = 1;
        }
        
        else if(practice.category[0] == X_LOW_PERCENT)
        {
            gameP->clearedStages = 0x1B;
        }
        else
        {
            gameP->clearedStages = 0x2B;
            gameP->collectables = 0x1020;
            gameP->armorParts = 0xF;
            gameP->busterType = 2;
        }
        
    }
    else if(gameP->stageId != 0) // 8 Mavericks
    {
        if (isRevist)
        {
            gameP->clearedStages = 0xDB;
            gameP->armorParts = 0xC;
            gameP->collectables = 0xE0D3;
            gameP->busterType = 1;
        }
        else if(gameP->player == 0)
        {
            if (practice.category[0] == X_HUNDO)
            {
                gameP->clearedStages = hundoXClearedTable[gameP->stageId - 1];
                gameP->collectables = hundoXCollectablesTable[gameP->stageId - 1];
                gameP->armorParts = hundoXArmorPartsTable[gameP->stageId - 1];

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 1)
                    {
                        gameP->armorParts = 8;
                    }
                    else if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0x41;
                    }
                    else if (gameP->stageId == 2)
                    {
                        gameP->collectables = 0x43;
                    }
                    else if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0xC053;
                    }
                    else if (gameP->stageId == 6)
                    {
                        gameP->collectables = 0xF0FF;
                        gameP->armorParts = 0xF;
                    }
                }
                
            }
            else if (practice.category[0] == X_ANY_PERCENT)
            {
                gameP->clearedStages = anyXClearedTable[gameP->stageId - 1];
                gameP->collectables = anyXCollectablesTable[gameP->stageId - 1];
                gameP->armorParts = anyXArmorPartsTable[gameP->stageId - 1];

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0x40;
                    }
                    else if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0x40D0;
                    }
                    else if (gameP->stageId == 6)
                    {
                        gameP->collectables = 0x70F0;
                    }
                }
                
            }
            else if (practice.category[0] == X_ANY_PERCENT_NEW)
            {
                gameP->clearedStages = anyNewXCleartedTable[gameP->stageId - 1];
                gameP->collectables = anyNewCollectablesTable[gameP->stageId - 1];
                gameP->collectables = anyNewXArmorPartsTable[gameP->stageId - 1];

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0x40;
                    }
                    else if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0x4050;
                    }
                    else if (gameP->stageId == 6)
                    {
                        gameP->collectables = 0x70F0;
                    }
                }
            }
            else if (practice.category[0] == X_LOW_PERCENT)
            {
                gameP->clearedStages = lowXCleartedTable[gameP->stageId - 1];
            }
            else //Ultimate Armor Any%
            {
                gameP->clearedStages = uaXClearedTable[gameP->stageId - 1];
                gameP->collectables = uaXCollectablesTable[gameP->stageId - 1];

                if (gameP->stageId != 6 || gameP->mid != 0)
                {
                    gameP->armorParts = 0xF;
                    gameP->busterType = 2;
                }

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0x1030;
                    }
                    else if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0x30F0;
                    }
                }
                
            }
            

            if ((gameP->armorParts & 4) != 0 && practice.category[0] != UA_PERCENT) // Buster Upgrade
            {
                gameP->busterType = 1;
            }
        }
        else
        {
            if (practice.category[1] == ZERO_HUNDO)
            {
                gameP->clearedStages = hundoZeroCleartedTable[gameP->stageId - 1];
                gameP->collectables = hundoZeroCollectablesTable[gameP->stageId - 1];

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 6)
                    {
                        gameP->collectables = 0x1028;
                    }
                    else if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0x103C;
                    }
                    else if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0x307D;
                    }
                    else if (gameP->stageId == 2)
                    {
                        gameP->collectables = 0xB0FF;
                    }
                }
            }
            else // Zero 100% (Stingray 1st)
            {
                gameP->clearedStages = hundoZeroStingrayClearedTable[gameP->stageId - 1];
                gameP->collectables = hundoZeroStingrayCollectablesTable[gameP->stageId - 1];

                if (gameP->mid != 0)
                {
                    if (gameP->stageId == 5)
                    {
                        gameP->collectables = 0x10;
                    }
                    else if (gameP->stageId == 6)
                    {
                        gameP->collectables = 0x3038;
                    }
                    else if (gameP->stageId == 8)
                    {
                        gameP->collectables = 0x30BC;
                    }
                    else if (gameP->stageId == 2)
                    {
                        gameP->collectables = 0xB0BE;
                    }
                    else if (gameP->stageId == 7)
                    {
                        gameP->collectables = 0xF0FF;
                    }
                }
                
            }
        }
    }
    if (gameP->maxHP == 32)
    {
        for (size_t i = 0; i < 8; i++)
        {
            if ((gameP->collectables & (1 << i)) != 0)
            {
                gameP->maxHP += 2;
            }
        }
    }
    if ((gameP->collectables & 0x1000) != 0 && gameP->stageId == 0xC)
    {
        gameP->tanksAmmo[0] = 20;
    }
    
    if (practice.playerFile != playerFileTable[game.player * 2 + game.cheatCodeFlag] && game.player == 0)
    {
        LoadPlayerFile();
    }
    SaveRestore();
}
void StageSelectResetClearedStages()
{
    game.clearedStages = 0;
}