#include <common.h>

#define Cursor titleP->mode3

static struct Title
{
    int8_t mode;
    int8_t mode2;
    int8_t mode3;
    int8_t mode4;

    int16_t timer;
};

void ShowPracticeTitleText();

void TitleLoop(struct Title *titleP)
{
    if (fadeDirection == 0)
    {
        if ((buttonsPressed & (PAD_UP + PAD_DOWN)) != 0)
        {
            PlaySound(0, 0xC, 0);
            Cursor ^= 2;
        }
        if ((buttonsPressed & (CONFIRM + PAD_START)) != 0)
        {
            titleP->mode2 = 2;
            PlaySound(0, 0x22, 0);
            FadeOut(8);
        }
        ShowPracticeTitleText();
    }
}

#undef Cursor