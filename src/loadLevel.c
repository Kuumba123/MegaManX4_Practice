#include <common.h>
#include <layer.h>
#include <gpu.h>
#include "practice.h"

#if BUILD == 561
#define UPDATECLUT *(uint8_t *)0x80166bb0
#define SCREENBACKUP *(uint32_t *)0x801441b4
#define BACKUPCLUT_ADDR 0x80141f70
#else
#define UPDATECLUT *(uint8_t *)0x80166c90
#define SCREENBACKUP *(uint32_t *)0x80144294
#define BACKUPCLUT_ADDR 0x80142050
#endif

extern uint8_t StageFileIdData[69];
extern uint8_t WeaponGetFileData[69];
extern uint8_t FinalWeaponFileData[69];
extern uint8_t MaverickFileData[69];
extern uint8_t MaverickClutFileData[69];
extern uint8_t StageSelectFileData[69];
extern uint8_t MegaClutFileData[69];
extern uint8_t ZeroClutFileData[69];

static struct Size
{
    uint8_t width;
    uint8_t height;
};

void * endFreeArcP;

extern struct Size StageLayoutSizeSettings[32];

extern uint8_t ScreenCountData[32];

void MemoryCopy(void *dest, const void *src, size_t size);

void LoadLevel(void)
{
    EndSong();
    practice.state.made = false;

    uint8_t bufferId;
    uint8_t fileId;

    /* For Weapon Get & Stage w/ Double/Iris */
    StageFileIdData[30] = WeaponGetFileData[game.player];
    StageFileIdData[22] = FinalWeaponFileData[game.player];

    /* For Maverick Intro */
    if ((game.stageId == 0xd) && (game.mid != 0))
    {
        uint32_t i = game.sceneFlags;
        StageFileIdData[27] = MaverickFileData[i];
        MegaClutFileData[27] = MaverickClutFileData[i];
        ZeroClutFileData[27] = MaverickClutFileData[i];
    }
    else
    {
        /* For Stage Select */
        StageFileIdData[26] = StageSelectFileData[game.player];
    }
    bufferId = 3;
    if (game.stageId == 0xe)
    {
        fileId = StageFileIdData[game.mid + 0x1c];
        bufferId = 0;
    }
    else
    {
        fileId = StageFileIdData[game.mid + game.stageId * 2];
    }
    ArcSeek((uint32_t)fileId, bufferId, 0);
    DrawLoad(game.stageId < 0xd, false);
    if (game.player == 0)
    {
        ArcSeek(MegaClutFileData[game.mid + game.stageId * 2], 1, 0);
    }
    else
    {
        ArcSeek(ZeroClutFileData[game.mid + game.stageId * 2], 1, 0);
    }
    DrawLoad(game.stageId < 0xd, true);

    layoutWidth = StageLayoutSizeSettings[game.stageId * 2 + game.mid].width;
    layoutHeight = StageLayoutSizeSettings[game.stageId * 2 + game.mid].height;
    layoutSize = layoutWidth * layoutHeight;

    // Backup Screen Data
    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    practice.state.screenSize = screenLength;
    SCREENBACKUP = freeArcP;
    MemoryCopy(freeArcP, *(uint32_t *)0x1F800008, screenLength);
    freeArcP = (void *)((uint8_t *)freeArcP + practice.state.screenSize);
    endFreeArcP = freeArcP;
    // Update Clut
    UPDATECLUT = 1;

    // Backup Clut
    MemoryCopy(BACKUPCLUT_ADDR, *((uint32_t *)0x1f800028), 0x1000);
}

#undef UPDATECLUT
#undef SCREENBACKUP
#undef BACKUPCLUT_ADDR