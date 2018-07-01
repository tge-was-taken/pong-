#pragma once

#include "../scenemanager.h"

#define ENT_SCORE_WIDTH 60
#define ENT_SCORE_HEIGHT 80

entEntity* entScoreCreate( s32 x, s32 y, const u32* score );
void entScoreRender( entEntity* pEntity );
void entScoreCollision( entEntity* pThis, entEntity* pOther );