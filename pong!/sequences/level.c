
#include "level.h"
#include "../gfx.h"
#include "../scenemanager.h"
#include "../entities/ball.h"
#include "../entities/paddle.h"
#include "../entities/score.h"
#include "../panelmanager.h"
#include "../panels/solidcolorpanel.h"
#include "../panels/dashedlinepanel.h"
#include "../panels/textpanel.h"
#include "../sequencemanager.h"
#include "../app.h"

#define seqLevelLog( FORMAT, ... )  dbgLogBase( seqLevel, FORMAT, ##__VA_ARGS__ );
#define MATCH_WIN_COUNT 5

typedef enum
{
    seqLevelState_None,
    seqLevelState_Init,
    seqLevelState_Match,
    seqLevelState_Result,
    seqLevelState_End,
} seqLevelState;

u32 gPlayerScores[ seqLevelPlayer_Count ];
static char gPlayerScoreTextBuffers[ seqLevelPlayer_Count ][ 4 ];
static entEntity* spBall;
static entEntity* gPaddles[ seqLevelPlayer_Count ];
static u32 sWaitTimer = 0;
static u32 sBallHitCount = 0;
static seqLevelState sState;

static void seqLevelResetBall()
{
    entBallContext* pContext = ( (entBallContext*)spBall->pContext );

    pContext->Speed = 10.0f;
    spBall->Position.X = 100;
    spBall->Position.Y = -20;
    spBall->Velocity.X = pContext->Speed;
    spBall->Velocity.Y = pContext->Speed;
    spBall->Direction.X = MATH_DIRECTION_RIGHT;
    spBall->Direction.Y = MATH_DIRECTION_DOWN;
}

static void seqLevelEntityCollisionCallback( entEntity* pA, entEntity* pB )
{
    if ( pA == spBall )
    {
        seqLevelLog( "Ball hit" );
        ++sBallHitCount;

        entBallContext* pContext = ( (entBallContext*)spBall->pContext );
        if ( ( sBallHitCount % 4 ) == 0 )
        {
            // Speed up ball every 4 hits
            pContext->Speed++;
        }
    }
}

void seqLevelInit( const void* pParams )
{
    utilUnused( pParams );
    seqLevelLog( "initializing" );

    // Start initializing
    sState = seqLevelState_Init;

    // Initialize score
    memZero( gPlayerScores, sizeof( gPlayerScores ) );
    gPlayerScoreTextBuffers[ seqLevelPlayer_1 ][ 0 ] = '0';
    gPlayerScoreTextBuffers[ seqLevelPlayer_2 ][ 0 ] = '0';

    // Create ball
    spBall = entBallCreate( 0, 0, 10.0f );
    seqLevelResetBall();

    // Create paddles
    gPaddles[ seqLevelPlayer_1 ] = entPaddleCreate( ( gScreenWidth - 20 ) - ENT_PADDLE_WIDTH, gScreenHeight / 2, true );
    gPaddles[ seqLevelPlayer_2 ] = entPaddleCreate( 20, gScreenHeight / 2, false );

    scnMgrSubscribeCollisionListener( seqLevelEntityCollisionCallback );

    // Build hud elements

    // Top rectangle
    pnlMgrAddPanel( pnlSolidColorPanelCreate( 0, 0, gScreenWidth, 30, 0xFFFFFFFF ) );

    // Seperator
    pnlMgrAddPanel( pnlDashedLinePanelCreate( ( gScreenWidth / 2 ) - 30, 40, 30, gScreenHeight, 0xFFFFFFFF, 30, 30) );

    // Bottom rectangle
    pnlMgrAddPanel( pnlSolidColorPanelCreate( 0, gScreenHeight - 30, gScreenWidth, 30, 0xFFFFFFFF ) );

    // Player 1 score
    pnlMgrAddPanel( pnlTextPanelCreate( ( gScreenWidth / 2 ) - ( gScreenWidth / 16 ) - 60, 40, 100, 100, gPlayerScoreTextBuffers[ seqLevelPlayer_1 ] ) );

    // Player 2 score
    pnlMgrAddPanel( pnlTextPanelCreate( ( gScreenWidth / 2 ) + ( gScreenWidth / 16 ), 40, 100, 100, gPlayerScoreTextBuffers[ seqLevelPlayer_2 ] ) );

    // Ready to begin the match
    sState = seqLevelState_Match;
}

static void seqLevelMatchUpdate();
static void seqLevelResultUpdate();

void seqLevelUpdate()
{
    if ( sWaitTimer )
    {
        --sWaitTimer;
        return;
    }

    // Update panel
    pnlMgrUpdate();

    switch ( sState )
    {
        case seqLevelState_Match:
            seqLevelMatchUpdate();
            break;

        case seqLevelState_Result:
            seqLevelResultUpdate();
            break;

        case seqLevelState_End:
            gQuitPending = true;
            break;
    }
}

static void seqLevelMatchUpdate()
{
    // Update entities
    scnMgrUpdate();

    // Check if ball is outside of the screen space on the X axis
    if ( !mathIsAxisInRange( spBall->Position.X, spBall->Width, 0, (s32)gScreenWidth ) )
    {
        // Check whose side it passed
        if ( spBall->Position.X > gScreenWidth / 2 )
        {
            // Update score
            sprintf( gPlayerScoreTextBuffers[ seqLevelPlayer_1 ], "%d", ++gPlayerScores[ seqLevelPlayer_1 ] );
        }
        else
        {
            // Left side
            sprintf( gPlayerScoreTextBuffers[ seqLevelPlayer_2 ], "%d", ++gPlayerScores[ seqLevelPlayer_2 ] );
        }

        if ( gPlayerScores[ seqLevelPlayer_1 ] >= MATCH_WIN_COUNT || gPlayerScores[ seqLevelPlayer_2 ] >= MATCH_WIN_COUNT )
        {
            pnlMgrDestroyPanels();
            sState = seqLevelState_Result;
        }

        // Reset ball position
        seqLevelResetBall();

        // Wait a second
        sWaitTimer = 60;
    }

}

static void seqLevelResultUpdate()
{
    seqLevelPlayer winner;
    if ( gPlayerScores[ seqLevelPlayer_1 ] >= MATCH_WIN_COUNT && gPlayerScores[ seqLevelPlayer_2 ] < MATCH_WIN_COUNT )
    {
        winner = seqLevelPlayer_1;
    }
    else
    {
        winner = seqLevelPlayer_2;
    }

    const char* pText;
    if ( winner == seqLevelPlayer_1 )
    {
        pText = "You lost :(";
    }
    else
    {
        pText = "You won!";
    }

    pnlMgrAddPanel( pnlTextPanelCreate( ( gScreenWidth / 2 ) - 100, ( gScreenHeight / 2 ) - 20, 100, 100, pText ) );

    sWaitTimer = 240;
    sState = seqLevelState_End;
}

void seqLevelRender()
{
    if ( sState == seqLevelState_Match )
        scnMgrRender();

    pnlMgrRender();
}

void seqLevelDestroy()
{
    seqLevelLog( "destroying" );
    scnMgrReset();
}