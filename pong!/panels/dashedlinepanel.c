
#include "dashedlinepanel.h"
#include "../gfx.h"

pnlDashedLinePanel* pnlDashedLinePanelCreate( s32 x, s32 y, u32 width, u32 height, u32 color, u32 dashHeight, u32 spacer )
{
    pnlDashedLinePanel* pPanel = (pnlDashedLinePanel*)pnlMgrCreatePanel( x, y, width, height, pnlDashedLinePanelUpdate, pnlDashedLinePanelRender, pnlDashedLinePanelDestroy, sizeof( pnlDashedLinePanel ) );
    pPanel->Color = color;
    pPanel->DashHeight = dashHeight;
    pPanel->Spacer = spacer;

    return pPanel;
}

void pnlDashedLinePanelUpdate( pnlDashedLinePanel* pPanel )
{
    utilUnused( pPanel );
}

void pnlDashedLinePanelRender( pnlDashedLinePanel* pPanel )
{
    u32 partCount = pPanel->Height / pPanel->Spacer;
    s32 yOffset = pPanel->Position.Y;

    for ( size_t i = 0; i < partCount; ++i )
    {
        if ( ( i % 2 ) == 1 )
        {
            // Skip every other part
            yOffset += pPanel->Spacer;
            continue;
        }

        gfxRenderRect( pPanel->Position.X, yOffset, pPanel->DashHeight, pPanel->Width, pPanel->Color );
        yOffset += pPanel->Spacer;
    }
}

void pnlDashedLinePanelDestroy( pnlDashedLinePanel* pPanel )
{
    utilUnused( pPanel );
}