#pragma once

#include "../scenemanager.h"

typedef struct
{
    f32 Speed;
} entBallContext;

entEntity* entBallCreate( s32 x, s32 y, f32 speed );
void entBallUpdate( entEntity* pEntity );
void entBallCollision( entEntity* pThis, entEntity* pOther );
void entBallDestroy( entEntity* pEntity );