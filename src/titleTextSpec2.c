#include <stdint.h>

static struct TitleText{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

struct TitleText gameStartTextData[] = {
    {160, 144, 2, 7} // GAME START
};
