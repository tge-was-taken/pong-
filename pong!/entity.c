#include "entity.h"
#include "gfx.h"

#include <SDL.h>

const char* gpEntEntityTypeToString[ entEntityType_Count ] =
{
    "None",
    "Ball",
    "Paddle",
    "Score"
};

void entDefaultUpdate( entEntity* pEntity )
{
    pEntity->Position.X += pEntity->Direction.X * pEntity->Velocity.X;
    pEntity->Position.Y += pEntity->Direction.Y * pEntity->Velocity.Y;
}

void entDefaultCollision( entEntity* pThis, entEntity* pOther )
{
    //pThis->Position.X -= ( -1 * pThis->Direction.X );
    //pThis->Position.Y -= ( -1 * pThis->Direction.Y );

    u32 maxX = ( pThis->Position.X + pThis->Width );
    if ( maxX >= pOther->Position.X )
    {
        u32 xOverlap = ( maxX - pOther->Position.X );
        pThis->Position.X -= -( xOverlap * pThis->Direction.X ) + -pThis->Direction.X;
    }

    //u32 maxY = ( pThis->Position.Y + pThis->Height );
    //if ( maxY >= pOther->Position.Y )
    //{
    //    u32 yOverlap = ( maxY - pOther->Position.Y );
    //    pThis->Position.Y -= -( yOverlap * pThis->Direction.Y ) + -pThis->Direction.Y;
    //}

    //pThis->Position.Y = maxY >= pOther->Position.Y ?
    //    pThis->Position.Y += ( pThis->Position.Y - ( maxY - pOther->Position.Y ) ) - pThis->Direction.Y :
    //    pThis->Position.Y;
}

void entDefaultRender( entEntity* pThis )
{
    gfxRenderRect( pThis->Position.X, pThis->Position.Y, pThis->Height, pThis->Width, pThis->Color );
}

void entDefaultDestroy( entEntity* pThis )
{
    utilUnused( pThis );
}