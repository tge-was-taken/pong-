#pragma once

#include "common.h"
#include "util/math.h"

typedef void( *pnlPanelUpdate )( struct pnlPanel* pPanel );
typedef void( *pnlPanelRender )( struct pnlPanel* pPanel );
typedef void( *pnlPanelDestroy )( struct pnlPanel* pPanel );

typedef struct
{
    u32 Id;
    mathVec2 Position;
    u32 Width;
    u32 Height;
    pnlPanelUpdate Update;
    pnlPanelRender Render;
    pnlPanelDestroy Destroy;
} pnlPanel;

void pnlMgrInit();
void pnlMgrUpdate();
void pnlMgrRender();
void pnlMgrDestroy();
pnlPanel* pnlMgrCreatePanel( s32 x, s32 y, u32 width, u32 height, pnlPanelUpdate update, pnlPanelRender draw, pnlPanelDestroy destroy, size_t size );
void pnlMgrAddPanel( pnlPanel* pPanel );
void pnlMgrRemovePanel( pnlPanel* pPanel );
void pnlMgrDestroyPanel( pnlPanel* pPanel );
void pnlMgrDestroyPanels();