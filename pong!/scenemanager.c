
#include "scenemanager.h"
#include "gfx.h"
#include "util/linkedlist.h"

#define scnMgrLog(FORMAT, ...)     dbgLogBase( scnMgr, FORMAT, ##__VA_ARGS__ )
#define scnMgrError(FORMAT, ...)   dbgErrorBase( scnMgr, FORMAT, ##__VA_ARGS__ )

typedef struct
{
    entEntity* pA;
    entEntity* pB;
} entEntityPair;

typedef struct
{
    u32 Id;
    entEntityCollisionFunc Callback;
} entCollisionListener;

#define MAX_ENTITY_COUNT ((size_t)8)
static u32 sNextId = 1;
static entEntity spEntities[ MAX_ENTITY_COUNT ];
static utilLinkedList sCollisionListeners;

static bool entCollisionTest( const entEntity* pA, const entEntity* pB )
{
    bool xOverlaps = mathIsValueInRange( pA->Position.X, pB->Position.X, pB->Position.X + pB->Width ) ||
        mathIsValueInRange( pB->Position.X, pA->Position.X, pA->Position.X + pA->Width );

    bool yOverlaps = mathIsValueInRange( pA->Position.Y, pB->Position.Y, pB->Position.Y + pB->Height ) ||
        mathIsValueInRange( pB->Position.Y, pA->Position.Y, pA->Position.Y + pA->Height );

    return xOverlaps && yOverlaps;
}

static void scnMgrUpdateEntities()
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == entEntityType_None || !spEntities[ i ].Update )
            continue;

        spEntities[ i ].Update( &spEntities[ i ] );
    }
}

static void scnMgrNotifyListener( utilLinkedListEntry* pEntry, void* pParams )
{
    entEntityCollisionFunc func = (entEntityCollisionFunc)( (entCollisionListener*)pEntry->pData )->Callback;
    entEntityPair* pEntityPair = (entEntityPair*)pParams;
    func( pEntityPair->pA, pEntityPair->pB );
}

static void scnMgrDetectCollisions()
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        entEntity* pA = &spEntities[ i ];

        if ( pA->Type == entEntityType_None || !pA->HasCollision )
            continue;

        for ( size_t j = 0; j < MAX_ENTITY_COUNT; ++j )
        {
            entEntity* pB = &spEntities[ j ];

            if ( j == i || pB->Type == entEntityType_None || !pB->HasCollision || !entCollisionTest( pA, pB ) )
                continue;

            scnMgrLog( "Collision between %s %d and %s %d", gpEntEntityTypeToString[ pA->Type ], pA->Id, gpEntEntityTypeToString[ pB->Type ], pB->Id );
            scnMgrLog( "Old position %f %f", pA->Position.X, pA->Position.Y );
            
            entEntityPair entityPair = { pA, pB };
            utilLinkedListForEach( &sCollisionListeners, scnMgrNotifyListener, &entityPair );

            if ( pA->Collision )
                pA->Collision( pA, pB );

            if ( pB->Collision )
                pB->Collision( pB, pA );

            scnMgrLog( "New position %f %f", pA->Position.X, pA->Position.Y );
        }
    }
}

u32 scnMgrSubscribeCollisionListener( entEntityCollisionFunc func )
{
    entCollisionListener* pListener = memAlloc( sizeof( entCollisionListener ) );
    pListener->Id = sNextId++;
    pListener->Callback = func;

    utilLinkedListAdd( &sCollisionListeners, pListener, memFree );
    return pListener->Id++;
}

static bool scnMgrCollisionListenerRemovePred( utilLinkedListEntry* pEntry, void* pParams )
{
    u32 id = *(u32*)pParams;
    return ( (entCollisionListener*)pEntry->pData )->Id == id;
}

void scnMgrUnsubscribeCollisionListener( u32 id )
{
    utilLinkedListRemovePred( &sCollisionListeners, scnMgrCollisionListenerRemovePred, &id );
}

void scnMgrInit()
{
    utilLinkedListInit( &sCollisionListeners );
}

void scnMgrUpdate()
{
    scnMgrUpdateEntities();
    scnMgrDetectCollisions();
}

void scnMgrRender()
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        entEntity* pEntity = &spEntities[ i ];

        if ( pEntity->Type == entEntityType_None || !pEntity->Render )
            continue;

        pEntity->Render( pEntity );
    }
}

void scnMgrForEachEntity( const scnMgrForEachEntityFunc func )
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == entEntityType_None )
            continue;

        func( &spEntities[ i ] );
    }
}

entEntity* scnMgrFindEntryByType( entEntityType type )
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == type )
            return &spEntities[ i ];
    }

    return NULL;
}

entEntity* scnMgrFindEntryById( u32 id )
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Id == id )
            return &spEntities[ i ];
    }

    return NULL;
}

entEntity* scnMgrCreateEntity( entEntityType type, s32 x, s32 y, s32 width, s32 height, u32 color, const entEntityUpdateFunc updateFunc, 
                               const entEntityCollisionFunc collisionFunc, const entEntityRenderFunc renderFunc, const entEntityDestroyFunc destroyFunc, 
                               const void* pContext )
{
    scnMgrLog( "creating entity type: %s x: %d y: %d width: %d height: %d", gpEntEntityTypeToString[ type ], x, y, width, height );

    size_t i;
    for ( i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == entEntityType_None )
            break;
    }

    dbgAssertMsg( i < MAX_ENTITY_COUNT, "Out of entity slots" );

    entEntity* pEntity = &spEntities[ i ];
    pEntity->Type = type;
    pEntity->Id = sNextId++;
    pEntity->Position.X = x;
    pEntity->Position.Y = y;
    pEntity->Width = width;
    pEntity->Height = height;
    pEntity->Color = color;
    pEntity->pContext = pContext;
    pEntity->Collision = collisionFunc;
    pEntity->Render = renderFunc;
    pEntity->Update = updateFunc;
    pEntity->Destroy = destroyFunc;
    pEntity->HasCollision = true;

    return pEntity;
}

void scnMgrDestroyEntity( const entEntity* pEntity )
{
    if ( pEntity->Destroy )
        pEntity->Destroy( pEntity );

    memZero( pEntity, sizeof( entEntity ) );
}

void scnMgrReset()
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == entEntityType_None )
            continue;

        if ( spEntities[ i ].Destroy )
            spEntities[ i ].Destroy( &spEntities[ i ] );

        memZero( &spEntities[ i ], sizeof( entEntity ) );
    }

    utilLinkedListDestroy( &sCollisionListeners );
    utilLinkedListInit( &sCollisionListeners );
}

void scnMgrDestroy()
{
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( spEntities[ i ].Type == entEntityType_None )
            continue;

        if ( spEntities[ i ].Destroy )
            spEntities[ i ].Destroy( &spEntities[ i ] );

        memZero( &spEntities[ i ], sizeof( entEntity ) );
    }

    utilLinkedListDestroy( &sCollisionListeners );
}