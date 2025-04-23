#include <common.h>

static struct Title
{
    int8_t mode;
    int8_t mode2;
    int8_t mode3;
    int8_t mode4;

    int16_t timer;
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void ShowPracticeTitleText()
{
    game.startMode = 0xA;
    DrawDebugText(11, 15, 0, "Practice\tBETA");
    DrawDebugText(21, 25, 2, "@PogChampGuy");
}
void TitlePressStartHook(struct Title *titleP)
{
    titleP->timer -= 1;
    if (titleP->timer == 1)
    {
        titleP->mode2 = 0xE;
        FadeOut(8);
    }
    ShowPracticeTitleText();
}