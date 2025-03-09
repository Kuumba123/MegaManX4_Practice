#include <common.h>
#include "practice.h"

#if BUILD == 561
#define FONT8X8_FILE_ID 63
#else
#define FONT8X8_FILE_ID 64
#endif

static uint16_t ScreenSectorOffset[] = {
    0x102,0x22,   //ST00
    0xD4,0x109,   //ST01
    0xF9,0xF4,    //ST02
    0xD0,0xE3,    //ST03
    0x105,0x130,  //ST04
    0xB0,0xC8,    //ST05
    0xB2,0xC3,    //ST06
    0xFA,0xB3,    //ST07
    0xBA,0xF0,    //ST08
    0x6D,0,       //ST09
    0xCC,0,       //ST0A
    0xEC,0x115,   //ST0B (0xEC/0xE2)
    0xC9,0xF4 //ST0C
};

static struct FileEntry
{
    uint32_t lba;
    uint32_t size;
    uint32_t unkown;
};

extern struct FileEntry fileData[163];

extern uint8_t StageFileIdData[69];

void LoadScreens()
{
    uint16_t fileId = StageFileIdData[game.stageId * 2 + game.mid];
    uint16_t offset = ScreenSectorOffset[game.stageId * 2 + game.mid];
    if (game.stageId == 0xB && game.mid == 0 && game.player != 0)
    {
        offset = 0xE2;
    }
    fileData[FONT8X8_FILE_ID].lba = fileData[fileId].lba + offset;
    fileData[FONT8X8_FILE_ID].size = practice.state.screenSize;
    void * temp = freeArcP;
    BinSeek(FONT8X8_FILE_ID,*(uint32_t *)0x1F800008);
    FileCollect();
    freeArcP = temp;
}

#undef FONT8X8_FILE_ID