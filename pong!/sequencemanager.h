#pragma once

#include "common.h"

typedef enum seqSequenceStateType
{
    seqSequenceStateType_Init,
    seqSequenceStateType_Title,
    seqSequenceStateType_Level,
    seqSequenceStateType_Count
} seqSequenceStateType;

void seqMgrInit();
void seqMgrDestroy();
void seqMgrTransition( seqSequenceStateType nextState, size_t paramsSize, const void* pParams );
void seqMgrUpdate();
void seqMgrRender();