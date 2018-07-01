
#include "score.h"
#include "../gfx.h"

#define SCORE_MAX 999
#define SCORE_MAX_DIGITS 3

static char spTextBuffer[ SCORE_MAX_DIGITS + 1 ];

entEntity* entScoreCreate( s32 x, s32 y, const u32* score )
{
    entEntity* pScore = scnMgrCreateEntity( entEntityType_Score, x, y, ENT_SCORE_WIDTH, ENT_SCORE_HEIGHT, 0xFFFFFFFF, NULL, entScoreCollision, entScoreRender, NULL, score );
    pScore->HasCollision = false;
    return pScore;
}

void entScoreRender( entEntity* pEntity )
{
    u32 score = mathMin( *(u32*)pEntity->pContext, SCORE_MAX );

    // Format score
    sprintf( spTextBuffer, "%d", score );

    // Render text
    gfxRenderText( pEntity->Position.X, pEntity->Position.Y, spTextBuffer );
}

void entScoreCollision( entEntity* pThis, entEntity* pOther )
{
    utilUnused( pThis );
    utilUnused( pOther );
}