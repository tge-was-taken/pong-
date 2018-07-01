#pragma once

#include "common.h"
#include <SDL.h>

extern u32 gScreenWidth;
extern u32 gScreenHeight;

bool gfxInit();
void gfxDestroy();
SDL_Texture* gfxLoadTextureFile( const char* pFilePath );
void gfxRenderRect( s32 x, s32 y, s32 height, s32 width, u32 color );
void gfxPresent();
void gfxClear();
void gfxSetScreenSize( u32 width, u32 height );
void gfxRenderText( s32 x, s32 y, const char* pText );