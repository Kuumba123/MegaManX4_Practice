#ifndef PRACTICE_H
#define PRACTICE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int timer;
    void * arcP;
    int32_t screenSize; //in bytes
    uint16_t rng;
    uint8_t bossFile;
    bool made;
}State;

typedef struct{
    int timer;
    State state;
    uint8_t category[2];
    bool keepRng;
    bool cancelXA;
    uint8_t playerFile;
}Practice;

extern Practice practice;

enum X_Categories {
    X_HUNDO,
    X_ANY_PERCENT,
    X_ANY_PERCENT_NEW,
    X_LOW_PERCENT,
    UA_PERCENT,
    X_CUSTOM
};

enum Zero_Categories {
    ZERO_HUNDO,
    ZERO_HUNDO_STRINGRAY,
    ZERO_CUSTOM
};

#endif