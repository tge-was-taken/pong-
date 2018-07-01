#pragma once

#include "../common.h"

typedef enum seqLevelPlayer
{
    seqLevelPlayer_1,
    seqLevelPlayer_2,
    seqLevelPlayer_Count,
} seqLevelPlayer;

extern u32 gPlayerScores[ seqLevelPlayer_Count ];

void seqLevelInit( const void* pParams );
void seqLevelUpdate();
void seqLevelRender();
void seqLevelDestroy();