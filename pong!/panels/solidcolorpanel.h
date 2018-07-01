#pragma once

#include "../common.h"
#include "../panelmanager.h"

typedef struct
{
    pnlPanel;
    u32 Color;
} pnlSolidColorPanel;

pnlSolidColorPanel* pnlSolidColorPanelCreate( s32 x, s32 y, u32 width, u32 height, u32 color );
void pnlSolidColorPanelUpdate( pnlSolidColorPanel* pPanel );
void pnlSolidColorPanelRender( pnlSolidColorPanel* pPanel );
void pnlSolidColorPanelDestroy( pnlSolidColorPanel* pPanel );