#include <stdint.h>

#if BUILD == 561
#define TX 248
#define TY 72
#define TS 0x1C
#define TC 4
#else
#define TX 0
#define TY 0
#define TS 0x1C
#define TC 4
#endif

static struct TitleText{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

struct TitleText titleTextData[] = {
    {TX, TY, TS, TC}, // ..
    {160, 248, 0x1D, 7}, // CONTINUE
    {160, 160, 0x1E, 7}, // OPTION
    {160, 128, 0x02, 6}, // ???
};

#undef TX
#undef TY
#undef TS
#undef TC