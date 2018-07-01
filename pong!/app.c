#include "app.h"
#include "gfx.h"
#include "sequencemanager.h"
#include "util/rand.h" 
#include "soundmanager.h"
#include "scenemanager.h"
#include "panelmanager.h"

bool gQuitPending = false;
int gFrameCounter = 0;

#define appLog(FORMAT, ...)     dbgLogBase( pong, FORMAT, ##__VA_ARGS__ )
#define appError(FORMAT, ...)   dbgErrorBase( pong, FORMAT, ##__VA_ARGS__ )

static bool appInit()
{
#if _DEBUG
    dbgInit();
#endif

    appLog( "initializing" );

    memMgrInit();

    if ( !gfxInit() )
    {
        appError( "gfxInit() failed" );
        return false;
    }

    utilRandInit();
    sndMgrInit();
    scnMgrInit();
    seqMgrInit();
    pnlMgrInit();

    appLog( "initialized successfully" );
    return true;
}

static void appDestroy()
{
    appLog( "destroying" );
    pnlMgrDestroy();
    seqMgrDestroy();
    scnMgrDestroy();
    sndMgrDestroy();
    gfxDestroy();
    memMgrDestroy();
}

static void appHandleEvents()
{
    SDL_Event e;

    while ( SDL_PollEvent( &e ) )
    {
        switch ( e.type )
        {
            case SDL_QUIT:
                gQuitPending = true;
                break;

            case SDL_WINDOWEVENT:
                if ( e.window.event == SDL_WINDOWEVENT_RESIZED )
                {
                    gfxSetScreenSize( e.window.data1, e.window.data2 );
                }
                break;

            default:
                break;
        }
    }
}

static void appUpdate()
{
    appHandleEvents();
    seqMgrUpdate();
}

static void appRender()
{
    gfxClear();
    seqMgrRender();
    gfxPresent();
}

static void appRun()
{
    while ( !gQuitPending )
    {
        appUpdate();
        appRender();
        ++gFrameCounter;
    }
}

int appMain( int argc, char* argv[] )
{
    utilUnused( argc );
    utilUnused( argv );

    if ( !appInit() )
    {
        appError( "appInit() failed" );
        assert( false );
        return 1;
    }

    appRun();
    appDestroy();

    return 0;
}