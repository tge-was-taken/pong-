#pragma once

#include "common.h"
#include "util/math.h"
#include <SDL_keycode.h>

typedef void( *entEntityUpdateFunc )( struct entEntity* pEntity );
typedef void( *entEntityCollisionFunc )( struct entEntity* pThis, struct entEntity* pOther );
typedef void( *entEntityRenderFunc )( struct entEntity* pThis );
typedef void( *entEntityDestroyFunc )( struct entEntity* pThis );

typedef enum entEntityType
{
    entEntityType_None,
    entEntityType_Ball,
    entEntityType_Paddle,
    entEntityType_Score,
    entEntityType_Count
} entEntityType;

extern const char* gpEntEntityTypeToString[ entEntityType_Count ];

typedef struct
{
    entEntityType Type;
    u32 Id;
    mathVec2 Position;
    mathVec2 Direction;
    mathVec2 Velocity;
    s32 Width;
    s32 Height;
    u32 Color;
    void* pContext;
    bool HasCollision;

    // Functions
    entEntityUpdateFunc Update;
    entEntityCollisionFunc Collision;
    entEntityRenderFunc Render;
    entEntityDestroyFunc Destroy;
} entEntity;

void entDefaultUpdate( entEntity* pEntity );
void entDefaultCollision( entEntity* pThis, entEntity* pOther );
void entDefaultRender( entEntity* pThis );
void entDefaultDestroy( entEntity* pThis );