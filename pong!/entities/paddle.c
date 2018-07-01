
#include "paddle.h"
#include "SDL.h"
#include "../gfx.h"
#include "../app.h"
#include "../util/rand.h"
#include "../soundmanager.h"

typedef struct
{
    bool IsPlayer;
} entPaddleContext;

entEntity* entPaddleCreate( s32 x, s32 y, bool isPlayer )
{
    entPaddleContext* pContext = memAlloc( sizeof( entPaddleContext ) );
    pContext->IsPlayer = isPlayer;

    entEntity* pBall = scnMgrCreateEntity( entEntityType_Paddle, x, y, ENT_PADDLE_WIDTH, ENT_PADDLE_HEIGHT, 0xFFFFFFFF, entPaddleUpdate, entPaddleCollision, entDefaultRender, entPaddleDestroy, pContext );
    return pBall;
}

static entEntity* entPaddleGetBall()
{
    static entEntity* pBall;

    if ( pBall == NULL )
        pBall = scnMgrFindEntryByType( entEntityType_Ball );

    return pBall;
}

static void entPaddlePlayerUpdate( entEntity* pEntity )
{
    pEntity->Velocity.Y = 0;

    const Uint8* kbState = SDL_GetKeyboardState( NULL );

    if ( kbState[ SDL_SCANCODE_UP ] )
    {
        pEntity->Direction.Y = MATH_DIRECTION_UP;
        pEntity->Velocity.Y = ENT_PADDLE_VELOCITY;
    }
    else if ( kbState[ SDL_SCANCODE_DOWN ] )
    {
        pEntity->Direction.Y = MATH_DIRECTION_DOWN;
        pEntity->Velocity.Y = ENT_PADDLE_VELOCITY;
    }
}

static void entPaddleAIUpdate( entEntity* pEntity )
{
    if ( ( gFrameCounter % utilRandBetween( 1, 4 ) ) != 0 )
        return;

    entEntity* pBall = entPaddleGetBall();

    if ( pBall->Position.Y < pEntity->Position.Y )
    {
        pEntity->Direction.Y = MATH_DIRECTION_UP;
        pEntity->Velocity.Y = ENT_PADDLE_VELOCITY;
    }
    else if ( pBall->Position.Y > pEntity->Position.Y )
    {
        pEntity->Direction.Y = MATH_DIRECTION_DOWN;
        pEntity->Velocity.Y = ENT_PADDLE_VELOCITY;
    }
    else
    {
        pEntity->Direction.Y = 0;
        pEntity->Velocity.Y = 0;
    }
}

void entPaddleUpdate( entEntity* pEntity )
{
    const entPaddleContext* pContext = (entPaddleContext*)pEntity->pContext;

    if ( pContext->IsPlayer )
    {
        entPaddlePlayerUpdate( pEntity );
    }
    else
    {
        entPaddleAIUpdate( pEntity );
    }

    entDefaultUpdate( pEntity );

    // Clamp position within screen
    pEntity->Position.Y = mathClamp( pEntity->Position.Y, 30, ( (s32)gScreenHeight - 30 ) - pEntity->Height );
}

void entPaddleCollision( entEntity* pThis, entEntity* pOther )
{
    sndMgrPlaySfx( "resources/beep.wav" );
    entDefaultCollision( pThis, pOther );
}

void entPaddleDestroy( entEntity* pThis )
{
    memFree( pThis->pContext );
}