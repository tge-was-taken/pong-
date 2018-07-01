
#include "gfx.h"
#include "scenemanager.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

u32 gScreenWidth = 1280;
u32 gScreenHeight = 720;
static SDL_Window* gpWindow = NULL;
static SDL_Renderer* gpRenderer = NULL;

#define gfxLog(FORMAT, ...)     dbgLogBase( gfx, FORMAT, ##__VA_ARGS__ )
#define gfxError(FORMAT, ...)   dbgErrorBase( gfx, FORMAT, ##__VA_ARGS__ )
#define gfxLogSdl2Error()       gfxLog( "SDL2 error %s in %s (%s:%d)",  SDL_GetError(), __FUNCTION__ "()", __FILE__, __LINE__ )
#define gfxLogSdl2ImgError()    gfxLog( "SDL_image error %s in %s (%s:%d)", IMG_GetError(), __FUNCTION__ "()", __FILE__, __LINE__ )
#define gfxLogSdl2TtfError()    gfxLog( "SDL2_ttf error %s in %s (%s:%d)", TTF_GetError(), __FUNCTION__ "()", __FILE__, __LINE__ )

bool gfxInit()
{
    gfxLog( "initializing" );

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        gfxLogSdl2Error();
        return false;
    }

    gpWindow = SDL_CreateWindow( "pong!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_SHOWN /* | SDL_WINDOW_RESIZABLE */ );
    if ( !gpWindow )
    {
        gfxLogSdl2Error();
        return false;
    }

    gpRenderer = SDL_CreateRenderer( gpWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( !gpRenderer )
    {
        gfxLogSdl2Error();
        return false;
    }

    SDL_SetRenderDrawColor( gpRenderer, 0x00, 0x00, 0x00, 0x00 );

    int imgFlags = IMG_INIT_PNG;
    if ( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        gfxLogSdl2ImgError();
        return false;
    }

    if ( TTF_Init() < 0 )
    {
        gfxLogSdl2TtfError();
        return false;
    }

    gfxLog( "initialized successfully" );
    return true;
}

void gfxDestroy()
{
    gfxLog( "destroying" );

    if ( gpRenderer )
    {
        SDL_DestroyRenderer( gpRenderer );
        gpRenderer = NULL;
    }

    if ( gpWindow )
    {
        SDL_DestroyWindow( gpWindow );
        gpWindow = NULL;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//SDL_Surface* gfxLoadImageFile( const char* pFilePath )
//{
//    SDL_Surface* pSurface = IMG_Load( pFilePath );
//    dbgAssert( pSurface != NULL, "IMG_Load() failed with %s\n", IMG_GetError() );
//
//    SDL_Surface* pOptimizedSurface = SDL_ConvertSurface( pSurface, gpScreen->format, 0 );
//    dbgAssert( pOptimizedSurface != NULL, "SDL_ConvertSurface() failed with %s\n", SDL_GetError() );
//
//    SDL_FreeSurface( pSurface );
//
//    return pOptimizedSurface;
//}

SDL_Texture* gfxLoadTextureFile( const char* pFilePath )
{
    gfxLog( "loading texture from file: %s", pFilePath );

    SDL_Surface* pSurface = IMG_Load( pFilePath );
    dbgAssertMsg( pSurface != NULL, "IMG_Load() failed with %s\n", IMG_GetError() );

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface( gpRenderer, pSurface );
    dbgAssertMsg( pTexture != NULL, "SDL_CreateTextureFromSurface() failed with %s\n", SDL_GetError() );

    SDL_FreeSurface( pSurface );
    return pTexture;
}

void gfxClear()
{
    SDL_SetRenderDrawColor( gpRenderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( gpRenderer );
}

void gfxPresent()
{
    SDL_RenderPresent( gpRenderer );
}

void gfxRenderRect( s32 x, s32 y, s32 height, s32 width, u32 color )
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;

    SDL_SetRenderDrawColor( gpRenderer,
                            color & 0xFF,
                            ( color >> 8 ) & 0xFF,
                            ( color >> 16 ) & 0xFF,
                            ( color >> 24 ) & 0xFF );

    SDL_RenderFillRect( gpRenderer, &rect );
}

TTF_Font* gfxGetFont()
{
    static TTF_Font* spFont = NULL;

    if ( spFont == NULL )
    {
        const char* pFontFilePath = "resources/arial.ttf";
        gfxLog( "Loading TTF font file: %s", pFontFilePath );
        spFont = TTF_OpenFont( pFontFilePath, 48 );
    }

    return spFont;
}

void gfxRenderText( s32 x, s32 y, const char* pText )
{
    // Render text to a surface using the font
    TTF_Font* pFont = gfxGetFont();
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, pText, color );

    // Convert the surface to a texture
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface( gpRenderer, pSurface );

    // We don't need the surface anymore.
    SDL_FreeSurface( pSurface );

    // Measure the width and height of the rendered text.
    int width, height;
    TTF_SizeText( pFont, pText, &width, &height );

    // Render the rendered text texture to the back buffer
    SDL_Rect rect = { .x = x,.y = y,.w = width,.h = height };
    SDL_RenderCopy( gpRenderer, pTexture, NULL, &rect );

    // We don't need the texture anymore
    SDL_DestroyTexture( pTexture );
}

void gfxSetScreenSize( u32 width, u32 height )
{
    gfxLog( "setting screen size to %dx%d", width, height );

    gScreenWidth = width;
    gScreenHeight = height;
    SDL_SetWindowSize( gpWindow, gScreenWidth, gScreenHeight );
}
