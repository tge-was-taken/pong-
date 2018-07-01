#pragma once

#include "../common.h"
#include "../panelmanager.h"

typedef struct
{
    pnlPanel;
    char* pText;
} pnlTextPanel;

pnlTextPanel* pnlTextPanelCreate( s32 x, s32 y, s32 width, s32 height, char* pText );
void pnlTextPanelUpdate( pnlTextPanel* pPanel );
void pnlTextPanelRender( pnlTextPanel* pPanel );
void pnlTextPanelDestroy( pnlTextPanel* pPanel );