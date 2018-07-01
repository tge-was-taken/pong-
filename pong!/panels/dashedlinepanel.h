#pragma once

#include "../common.h"
#include "../panelmanager.h"

typedef struct
{
    pnlPanel;
    u32 Color;
    u32 DashHeight;
    u32 Spacer;
} pnlDashedLinePanel;

pnlDashedLinePanel* pnlDashedLinePanelCreate( s32 x, s32 y, u32 width, u32 height, u32 color, u32 dashHeight, u32 spacer );
void pnlDashedLinePanelUpdate( pnlDashedLinePanel* pPanel );
void pnlDashedLinePanelRender( pnlDashedLinePanel* pPanel );
void pnlDashedLinePanelDestroy( pnlDashedLinePanel* pPanel );