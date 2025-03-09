#include <common.h>

void StateCheck(Game * gameP);

static void (*mode_A_Table[3])(Game*) = {StateCheck,StateCheck,StateCheck};