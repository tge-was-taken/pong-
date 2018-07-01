
#include "textpanel.h"
#include "../gfx.h"

pnlTextPanel* pnlTextPanelCreate( s32 x, s32 y, s32 width, s32 height, char* pText )
{
    pnlTextPanel* pPanel = (pnlTextPanel*)pnlMgrCreatePanel( x, y, width, height, pnlTextPanelUpdate, pnlTextPanelRender, pnlTextPanelDestroy, sizeof( pnlTextPanel ) );
    pPanel->pText = pText;

    return pPanel;
}

void pnlTextPanelUpdate( pnlTextPanel* pPanel )
{
    utilUnused( pPanel );
}

void pnlTextPanelRender( pnlTextPanel* pPanel )
{
    gfxRenderText( pPanel->Position.X, pPanel->Position.Y, pPanel->pText );
}

void pnlTextPanelDestroy( pnlTextPanel* pPanel )
{
    utilUnused( pPanel );
}