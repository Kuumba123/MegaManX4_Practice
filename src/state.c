#include <common.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

#define VariableSetsCount 23
#define BuffersCount 5

#if BUILD == 561
#define EARTH_SIGMA_ID 0x81
#define ST0C_1_ARC_ID 0x6D
#define UPDATECLUT *(uint8_t *)0x80166bb0
#define SCREENBACKUP *(uint32_t *)0x801441b4
#define RNG *(uint16_t *)0x8002b73c
#define BOSSFILE *(uint8_t *)0x80171ea8
#define STARTSELECT_FLAG *(uint32_t *)0x8013bd44
#define VABP *(int *)0x80141f38
#else
#define EARTH_SIGMA_ID 0x80
#define ST0C_1_ARC_ID 0x6C
#define UPDATECLUT *(uint8_t *)0x80166c90
#define SCREENBACKUP *(uint32_t *)0x80144294
#define RNG *(uint16_t *)0x8013e3c8
#define BOSSFILE *(uint8_t *)0x80171f60
#define STARTSELECT_FLAG *(uint32_t *)0x8013be24
#define VABP *(int *)0x80142018
#endif

#define FADE_F *(uint16_t *)0x801F8200

extern void *clutPointer;
extern Enemy *enemyDataPointers[];
extern uint8_t loadState; /*1=loading*/

#if BUILD == 561
static void (*mode_A_Table[3])(Game *) = {0x8001ff8c, 0x80020060, 0x80020368};
#else
static void (*mode_A_Table[3])(Game *) = {0x8001ffe8, 0x800200a8, 0x800203b0};
#endif

void LoadCompressedImage(Object *objP, int16_t x, int16_t y);
void LoadBossRefightsArc();

#if BUILD == 561
#define TEXTCLUT_BACKUP_ADDR 0x801397e4
#define RIDE_ARMOR_ADDR 0x80173a30
#define TEXT_INFO_ADDR 0x8013bc28
#define TEXT_EXTRA_ADDR 0x801396c8
#define BRIGHT_ADDR 0x8013e1bc
#define M_BRIGHT_ADDR 0x80175ea0
#define EM_BRIGHT_ADDR 0x80141be4
#define TBL_BRIGHT_ADDR 0x8013e188
#define AFTER_IMG_ADDR 0x80141ab0
#define BOSS_VARS_ADDR 0x8013b828 /*(0x148 bytes)*/
#else
#define TEXTCLUT_BACKUP_ADDR 0x801398c8
#define RIDE_ARMOR_ADDR 0x80173ae8
#define TEXT_INFO_ADDR 0x8013bd08
#define TEXT_EXTRA_ADDR 0x801397e8
#define BRIGHT_ADDR 0x8013e29c
#define M_BRIGHT_ADDR 0x80175f58
#define EM_BRIGHT_ADDR 0x80141cc4
#define TBL_BRIGHT_ADDR 0x8013e268
#define AFTER_IMG_ADDR 0x80141b90
#define BOSS_VARS_ADDR 0x8013b908
#endif

static void *freeAddress[] = {
    0x801f6000, // Demo Buffer
    0x801F8304, // Just before Thread Stack Memory
    0x801EFC8C, // End of ARC buffer
    0x8000A000, // Kernal RAM
    0x8000C000  // Kernal RAM
};
static int16_t freeAddressSizes[] = {
    0x2000,
    0x65FC,
    0x3374,
    0x1900,
    0x1F00};

static void *readAddress[] = {
    -1,                   // Clut (dynamicly set based off of CLUT Pointer)
    TEXTCLUT_BACKUP_ADDR, // temporary store of CLUT
    WEAPON_OBJECT_ADDR,   // Weapon Objects
    ENEMY_OBJECT_ADDR,    // Main Objects
    SHOT_OBJECT_ADDR,     // Shot Objects
    VISUAL_OBJECT_ADDR,   // Visual Objects
    EFFECT_OBJECT_ADDR,   // Effect Objects
    ITEM_OBJECT_ADDR,     // Item Objects
    MISC_OBJECT_ADDR,     // Misc Objects
    LAYER_OBJECT_ADDR,    // Layer Objects
    QUAD_OBJECT_ADDR,     // Quad Objects
    &mega,                // Mega
    &bgLayers,            // BG Layers
    &game,                // Game Info
    RIDE_ARMOR_ADDR,      // Ride Armor
    TEXT_INFO_ADDR,       // Text Info
    TEXT_EXTRA_ADDR,      // Extra Text Info
    BRIGHT_ADDR,          // Brightness stuff
    M_BRIGHT_ADDR,        // More Brightness stuff
    EM_BRIGHT_ADDR,       // Also More Brightness stuff
    TBL_BRIGHT_ADDR,      // Brightness Table thing
    AFTER_IMG_ADDR,       // After Images
    BOSS_VARS_ADDR};

static int16_t addressesSize[] = { // 0x9BB0
    0x1000,
    0x40,
    WEAPON_OBJECT_SLOT_SIZES *WEAPON_OBJECT_SLOTS,
    ENEMY_OBJECT_SLOT_SIZES *ENEMY_OBJECT_SLOTS,
    SHOT_OBJECT_SLOT_SIZES *SHOT_OBJECT_SLOTS,
    VISUAL_OBJECT_SLOT_SIZES *VISUAL_OBJECT_SLOTS,
    EFFECT_OBJECT_SLOT_SIZES *EFFECT_OBJECT_SLOTS,
    ITEM_OBJECT_SLOT_SIZES *ITEM_OBJECT_SLOTS,
    MISC_OBJECT_SLOT_SIZES *MISC_OBJECT_SLOTS,
    LAYER_OBJECT_SLOT_SIZES *LAYER_OBJECT_SLOTS,
    QUAD_OBJECT_SLOTS_SIZES *QUAD_OBJECT_SLOTS,
    0xE8,
    0xFC,
    0x64,
    0xB0,
    0x18,
    0x164,
    4,
    4,
    4,
    0x28,
    0x120,
    0x148};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SwapWeaponTexturesClut(Mega *megaP);

void CheckPointCheck(Game *gameP);

void MemoryCopy(void *dest, const void *src, size_t size)
{
    // Ensure that the size is a multiple of 4 bytes
    size_t num_4byte_blocks = size / 4;

    uint32_t *d = (uint32_t *)dest;
    const uint32_t *s = (const uint32_t *)src;

    for (size_t i = 0; i < num_4byte_blocks; i++)
    {
        d[i] = s[i];
    }
}
void SaveState()
{
    ThreadSleep(10);
    readAddress[0] = clutPointer;

    // backup enemy data other vars
    Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
    uint8_t *p2 = 0x1F800128;
    while ((p->type & 0xF) != 0xF)
    {
        *p2 = p->spawned;
        p += 1;
        p2 += 1;
    }

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(freeP, srcAddr, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;

            if (freeId > BuffersCount - 1)
            {
                printf("ERROR: went past MAX buffer counts: %X\n", BuffersCount);
                return;
            }

            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(freeP, srcAddr, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    practice.state.timer = practice.timer;
    practice.state.rng = RNG;
    practice.state.arcP = freeArcP;
    practice.state.bossFile = BOSSFILE;

    practice.state.made = true; // Mark State as Made

    MemoryCopy(SCREENBACKUP, *(uint32_t *)0x1F800008, practice.state.screenSize);
}
void LoadState()
{
    ThreadSleep(10);

    // restore enemy data
    {
        Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
        uint8_t *p2 = 0x1F800128;
        while ((p->type & 0xF) != 0xF)
        {
            p->spawned = *p2;
            p->type = p->type & 0xF;
            p += 1;
            p2 += 1;
        }
    }

    uint8_t pastPoint = game.point;
    uint8_t pastFile = BOSSFILE;

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(srcAddr, freeP, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;
            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(srcAddr, freeP, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    if (game.startingSong != 0)
    {
        EndSong();
    }

    if (practice.keepRng)
    {
        RNG = practice.state.rng;
    }

    practice.timer = practice.state.timer;
    freeArcP = practice.state.arcP;
    BOSSFILE = practice.state.bossFile;

    bgLayers[0].update = true;
    bgLayers[1].update = true;
    bgLayers[2].update = true;

    if (game.stageId == 0xC && game.mid == 0)
    {
        if (game.point >= 2 && game.point <= 8 && game.point != pastPoint)
        {
            EndSong();
            LoadBossRefightsArc();
            ThreadSleep(2);
            FadeIn(10);
            while (fadeDirection != 0)
            {
                ThreadSleep(1);
            }
        }
        else
        {
            BOSSFILE = 0;
        }
    }
    else if (game.stageId == 0xC && game.mid != 0)
    {
        if (BOSSFILE == 0 && pastFile != BOSSFILE)
        {
            EndSong();
            ArcSeek(ST0C_1_ARC_ID, 3, 0);
            DrawLoad(0, 0);
            freeArcP = practice.state.arcP;
        }
        else if (BOSSFILE != 0 && pastFile != BOSSFILE)
        {
            EndSong();
            ArcSeek(EARTH_SIGMA_ID, 4, VABP);
            DrawLoad(0, 0);
            freeArcP = practice.state.arcP;
        }
    }
    UPDATECLUT = 1;
    STARTSELECT_FLAG = 1;
    mega.newAnimeF = -1;
    LoadCompressedImage((Object *)&mega, 320, 0);
    if (mega.player == 0)
    {
        SwapWeaponTexturesClut(&mega);
    }
    MemoryCopy(*(uint32_t *)0x1F800008, SCREENBACKUP, practice.state.screenSize);
}
void StateCheck(Game *gameP)
{
    int time = practice.timer;
    practice.timer += 1;
    int totalSeconds = time / 60;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int frames = time % 60;

    DrawDebugText(5, 3, 0, "%2d:%2d:%2d", minutes, seconds, frames);

    if (loadState == 1)
    {
        DrawDebugText(5, 4, 2, "(Loading)");
    }

    if (loadState != 1 && FADE_F == 0 && gameP->mode2 != 2)
    {
        if ((buttonsPressed & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_SELECT | PAD_LEFT | PAD_RIGHT | PAD_TRIANGLE)) != 0)
        {
            if ((buttonsHeld & (PAD_R2 + PAD_SELECT)) == (PAD_R2 + PAD_SELECT))
            {
                SaveState();
            }
            else if ((buttonsHeld & (PAD_L2 + PAD_SELECT)) == (PAD_L2 + PAD_SELECT) && practice.state.made)
            {
                LoadState();
            }
            else
            {
                CheckPointCheck(gameP);
            }
        }
    }
    mode_A_Table[gameP->mode2](gameP);
}

#undef EARTH_SIGMA_ID
#undef ST0C_1_ARC_ID
#undef UPDATECLUT
#undef SCREENBACKUP
#undef RNG
#undef BOSSFILE
#undef STARTSELECT_FLAG
#undef VABP

#undef FADE_F

#undef TEXTCLUT_BACKUP_ADDR
#undef RIDE_ARMOR_ADDR
#undef TEXT_INFO_ADDR
#undef TEXT_EXTRA_ADDR
#undef BRIGHT_ADDR
#undef M_BRIGHT_ADDR
#undef EM_BRIGHT_ADDR
#undef TBL_BRIGHT_ADDR
#undef AFTER_IMG_ADDR
#undef BOSS_VARS_ADDR