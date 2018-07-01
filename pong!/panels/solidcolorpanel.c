
#include "solidcolorpanel.h"
#include "../gfx.h"

pnlSolidColorPanel* pnlSolidColorPanelCreate( s32 x, s32 y, u32 width, u32 height, u32 color )
{
    pnlSolidColorPanel* pPanel = (pnlSolidColorPanel*)pnlMgrCreatePanel( x, y, width, height, pnlSolidColorPanelUpdate, pnlSolidColorPanelRender, pnlSolidColorPanelDestroy, sizeof( pnlSolidColorPanel ) );
    pPanel->Color = color;

    return pPanel;
}

void pnlSolidColorPanelUpdate( pnlSolidColorPanel* pPanel )
{
    utilUnused( pPanel );
}

void pnlSolidColorPanelRender( pnlSolidColorPanel* pPanel )
{
    gfxRenderRect( pPanel->Position.X, pPanel->Position.Y, pPanel->Height, pPanel->Width, pPanel->Color );
}

void pnlSolidColorPanelDestroy( pnlSolidColorPanel* pPanel )
{
    utilUnused( pPanel );
}