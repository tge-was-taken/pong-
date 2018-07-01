#pragma once

#include "entity.h"

typedef void( *scnMgrForEachEntityFunc )( const entEntity* pEntity );

void scnMgrInit();
void scnMgrUpdate();
void scnMgrRender();
entEntity* scnMgrCreateEntity( entEntityType type, s32 x, s32 y, s32 width, s32 height, u32 color, const entEntityUpdateFunc updateFunc,
                               const entEntityCollisionFunc collisionFunc, const entEntityRenderFunc renderFunc, const entEntityDestroyFunc destroyFunc,
                               const void* pContext );
void scnMgrDestroyEntity( const entEntity* pEntity );
void scnMgrDestroy();
void scnMgrReset();
void scnMgrForEachEntity( const scnMgrForEachEntityFunc func );
entEntity* scnMgrFindEntryByType( entEntityType type );
entEntity* scnMgrFindEntryById( u32 id );
u32 scnMgrSubscribeCollisionListener( entEntityCollisionFunc func );
void scnMgrUnsubscribeCollisionListener( u32 id );