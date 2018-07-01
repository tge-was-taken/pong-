
#include "sequencemanager.h"
#include "sequences/level.h"

#define seqLog(FORMAT, ...)     dbgLogBase( seq, FORMAT, ##__VA_ARGS__ )
#define seqError(FORMAT, ...)   dbgErrorBase( seq, FORMAT, ##__VA_ARGS__ )

typedef void( *seqSequenceStateInitFunc )( const void* pParams );
typedef void( *seqSequenceStateUpdateFunc )();
typedef void( *seqSequenceStateRenderFunc )( );
typedef void( *seqSequenceStateDestroyFunc )();

typedef struct
{
    char* pName;
    size_t ParamsSize;
    seqSequenceStateInitFunc Init;
    seqSequenceStateUpdateFunc Update;
    seqSequenceStateRenderFunc Render;
    seqSequenceStateDestroyFunc Destroy;
} seqSequenceState;

static seqSequenceState seqSequenceTable[ seqSequenceStateType_Count ] =
{
    //{ "Init", 0, NULL, NULL, NULL },
    //{ "Title", 0, NULL, NULL, NULL },
    { "Level", 0, seqLevelInit, seqLevelUpdate, seqLevelRender, seqLevelDestroy },
};

static seqSequenceState* spCurrentState = &seqSequenceTable[ 0 ];

void seqMgrInit()
{
    seqLog( "initializing" );
    spCurrentState->Init( NULL );
}

void seqMgrDestroy()
{
    seqLog( "destroying" );
    spCurrentState->Destroy();
}

void seqMgrTransition( seqSequenceStateType nextState, size_t paramsSize, const void* pParams )
{   
    dbgAssertMsg( nextState < seqSequenceStateType_Count, "Invalid sequence type" );

    seqSequenceState* pNextState = &seqSequenceTable[ nextState ];
    seqLog( "Transition from state %s to %s", spCurrentState->pName, pNextState->pName );

    // End current state
    if ( spCurrentState->Destroy )
        spCurrentState->Destroy();

    // Set new state
    spCurrentState = &seqSequenceTable[ nextState ];
    dbgAssertMsg( spCurrentState->ParamsSize == paramsSize, "Invalid params size" );
    spCurrentState->Init( pParams );
}

void seqMgrUpdate()
{
    // Update state
    if ( spCurrentState->Update )
        spCurrentState->Update();
}

void seqMgrRender()
{
    // Update state
    if ( spCurrentState->Render )
        spCurrentState->Render();
}