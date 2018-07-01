
#include "panelmanager.h"

#define pnlMgrLog(FORMAT, ...)     dbgLogBase( pnlMgr, FORMAT, ##__VA_ARGS__ )
#define pnlMgrError(FORMAT, ...)   dbgErrorBase( pnlMgr, FORMAT, ##__VA_ARGS__ )

#define MAX_PANEL_COUNT 8
static u32 sNextId = 1;
static pnlPanel* spPanels[ MAX_PANEL_COUNT ];

void pnlMgrInit()
{
    pnlMgrLog( "initializing" );
}

void pnlMgrUpdate()
{
    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        pnlPanel* pPanel = spPanels[ i ];
        if ( pPanel && pPanel->Update )
            pPanel->Update( pPanel );
    }
}

void pnlMgrRender()
{
    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        pnlPanel* pPanel = spPanels[ i ];
        if ( pPanel && pPanel->Render )
            pPanel->Render( pPanel );
    }
}

void pnlMgrDestroy()
{
    pnlMgrLog( "destroying" );

    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        pnlPanel* pPanel = spPanels[ i ];
        if ( pPanel )
        {
            pnlMgrDestroyPanel( pPanel );
            spPanels[ i ] = NULL;
        }
    }
}

pnlPanel* pnlMgrCreatePanel( s32 x, s32 y, u32 width, u32 height, pnlPanelUpdate update, pnlPanelRender draw, pnlPanelDestroy destroy, size_t size )
{
    pnlPanel* pPanel = memAlloc( size );
    pPanel->Id = sNextId++;
    pPanel->Position.X = x;
    pPanel->Position.Y = y;
    pPanel->Width = width;
    pPanel->Height = height;
    pPanel->Update = update;
    pPanel->Render = draw;
    pPanel->Destroy = destroy;

    pnlMgrLog( "created new panel: id: %d x: %d: y: %d w: %d h: %d", pPanel->Id, pPanel->Position.X, pPanel->Position.Y, pPanel->Width, pPanel->Height );

    return pPanel;
}

void pnlMgrAddPanel( pnlPanel* pPanel )
{
    pnlMgrLog( "adding panel id: %d", pPanel->Id );

    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        pnlPanel* pCurElement = spPanels[ i ];
        if ( !pCurElement )
        {
            spPanels[ i ] = pPanel;
            return;
        }
    }

    dbgAssertMsg( false, "Out of panel slots" );
}

void pnlMgrRemovePanel( pnlPanel* pPanel )
{
    pnlMgrLog( "removing panel id: %d", pPanel->Id );

    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        if ( spPanels[ i ] == pPanel )
            spPanels[ i ] = NULL;
    }
}

void pnlMgrDestroyPanel( pnlPanel* pPanel )
{
    pnlMgrLog( "destroying panel id: %d", pPanel->Id );

    if ( pPanel->Destroy )
        pPanel->Destroy( pPanel );

    memFree( pPanel );
}

void pnlMgrDestroyPanels()
{
    for ( size_t i = 0; i < MAX_PANEL_COUNT; ++i )
    {
        pnlPanel* pPanel = spPanels[ i ];
        if ( pPanel )
        {
            pnlMgrDestroyPanel( pPanel );
            spPanels[ i ] = NULL;
        }
    }
}