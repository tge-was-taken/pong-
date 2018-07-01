
#include "ball.h"
#include "../gfx.h"
#include "../util/rand.h"
#include "../dbg.h"

entEntity* entBallCreate( s32 x, s32 y, f32 speed )
{
    entBallContext* pContext = memAlloc( sizeof( entBallContext ) );
    pContext->Speed = speed;

    entEntity* pBall = scnMgrCreateEntity( entEntityType_Ball, x, y, 20, 20, 0xFFFFFFFF, entBallUpdate, entBallCollision, entDefaultRender, entBallDestroy, pContext );
    return pBall;
}

void entBallUpdate( entEntity* pEntity )
{
    entDefaultUpdate( pEntity );

    if ( !mathIsAxisInRange( pEntity->Position.Y, pEntity->Height, 30, ( (s32)gScreenHeight - 30 )  ) )
    {
        pEntity->Direction.Y = -pEntity->Direction.Y;
        pEntity->Position.Y = mathClamp( pEntity->Position.Y, 30, ( (s32)gScreenHeight - 30 ) - pEntity->Height );
    }

    // For bouncy fun
    //if ( !mathIsAxisInRange( pEntity->Position.X, pEntity->Width, 0, (s32)gScreenWidth ) )
    //{
    //    pEntity->Direction.X = -pEntity->Direction.X;
    //    pEntity->Position.X = mathClamp( pEntity->Position.X, 0, (s32)gScreenWidth - pEntity->Width );
    //}
}

void entBallCollision( entEntity* pThis, entEntity* pOther )
{
    entBallContext* pContext = ( entBallContext* )pThis->pContext;

    // Invert direction
    pThis->Direction.X = -pThis->Direction.X;

    if ( mathIsValueInRange( pThis->Position.Y + pThis->Height, pOther->Position.Y, pOther->Position.Y + ( pOther->Height / 2 ) ) )
    {
        // Hit upper half, move up
        pThis->Direction.Y = MATH_DIRECTION_UP;
    }
    else
    {
        // Hit lower half, move down
        pThis->Direction.Y = MATH_DIRECTION_DOWN;
    }

    pThis->Velocity.Y = pContext->Speed + utilRandBetween( -2, 4 );
    pThis->Velocity.X = pContext->Speed + utilRandBetween( -2, 4 );

    entDefaultCollision( pThis, pOther );
}

void entBallDestroy( entEntity* pEntity )
{
    memFree( pEntity->pContext );
}