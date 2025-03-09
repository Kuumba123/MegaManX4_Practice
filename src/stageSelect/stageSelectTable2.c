#include <common.h>

void StageSelectDetermine(Game * gameP);
void ConfigPage(Game *gameP);
void StageSelectArea(Game *gameP);

static void * selectTable[] = {StageSelectDetermine, ConfigPage, StageSelectArea};