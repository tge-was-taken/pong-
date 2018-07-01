#pragma once

#include "../scenemanager.h"

#define ENT_PADDLE_WIDTH 20
#define ENT_PADDLE_HEIGHT 80
#define ENT_PADDLE_VELOCITY 10.0f
#define ENT_PADDLE_AI_VELOCITY_BIAS 0.25f

entEntity* entPaddleCreate( s32 x, s32 y, bool isPlayer );
void entPaddleUpdate( entEntity* pEntity );
void entPaddleCollision( entEntity* pThis, entEntity* pOther );
void entPaddleDestroy( entEntity* pThis );