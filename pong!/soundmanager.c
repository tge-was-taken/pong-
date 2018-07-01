
#include "soundmanager.h"
#include "util/dictionary.h"

#include <SDL_mixer.h>

#define sndMgrLog(FORMAT, ...)     dbgLogBase( sndMgr, FORMAT, ##__VA_ARGS__ )
#define sndMgrError(FORMAT, ...)   dbgErrorBase( sndMgr, FORMAT, ##__VA_ARGS__ )

typedef struct
{
    const void* pData;
    const char* pFilePath;
} sndMgrSound;

static sndMgrSound spCurMusic;
static sndMgrSound spChunks[ MIX_CHANNELS ];
static utilDict spChunkCache;

static void sndMgrDestroyMusic( sndMgrSound* pSound )
{
    Mix_FreeMusic( pSound->pData );
    memZero( pSound, sizeof( sndMgrSound ) );
}

static void sndMgrDestroySfx( sndMgrSound* pSound )
{
    Mix_FreeChunk( pSound->pData );
    memZero( &pSound, sizeof( sndMgrSound ) );
}

static bool sndMgrIsSfxPlaying( const char* pFilePath )
{
    for ( size_t i = 0; i < MIX_CHANNELS; ++i )
    {
        if ( spChunks[i].pData && strcmp( spChunks[ i ].pFilePath, pFilePath ) == 0 )
            return true;
    }

    return false;
}

static void sndMgrChannelFinishedCallback( s32 channel )
{
    memZero( &spChunks[ channel ], sizeof( sndMgrSound ) );
}

static void sndMgrMusicFinishedCallback()
{
    sndMgrDestroyMusic( &spCurMusic );
}

bool sndMgrInit()
{
    sndMgrLog( "initializing" );

    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096 ) < 0 )
    {
        sndMgrError( "Mix_OpenAudio failed: %s", Mix_GetError() );
        return false;
    }

    Mix_HookMusicFinished( sndMgrMusicFinishedCallback );
    Mix_ChannelFinished( sndMgrChannelFinishedCallback );

    utilDictInit( &spChunkCache, 4 );

    return true;
}

void sndMgrDestroy()
{
    sndMgrLog( "destroying" );

    Mix_CloseAudio();

    if ( spCurMusic.pData )
    {
        sndMgrDestroyMusic( &spCurMusic );
    }

    for ( s32 i = 0; i < MIX_CHANNELS; ++i )
    {
        if ( spChunks[ i ].pData )
        {
            sndMgrDestroySfx( &spChunks[ i ] );
        }
    }

    utilDictDestroy( &spChunkCache );
}

void sndMgrPlayMusic( const char* pFilePath )
{
    sndMgrLog( "Play music: %s", pFilePath );

    if ( spCurMusic.pData )
    {
        sndMgrDestroyMusic( &spCurMusic );
    }

    spCurMusic.pData = Mix_LoadMUS( pFilePath );
    spCurMusic.pFilePath = pFilePath;

    Mix_PlayMusic( spCurMusic.pData, -1 ); // todo: looping
}

void sndMgrPauseMusic()
{
    sndMgrLog( "Pause music" );
    Mix_PauseMusic();
}

void sndMgrResumeMusic()
{
    sndMgrLog( "Resume music" );
    Mix_ResumeMusic();
}

void sndMgrStopMusic()
{
    sndMgrLog( "Stop music" );
    Mix_HaltMusic();

    if ( spCurMusic.pData )
    {
        sndMgrDestroyMusic( &spCurMusic );
    }
}

bool sndMgrIsPlayingMusic()
{
    return Mix_PlayingMusic();
}

void sndMgrPlaySfx( const char* pFilePath )
{
    sndMgrPlaySfxLooped( pFilePath, 0 );
}

static s32 sndMgrFindNextAvailableChannel()
{
    for ( s32 i = 0; i < MIX_CHANNELS; ++i )
    {
        if ( spChunks[ i ].pData == NULL )
            return i;
    }

    return -1;
}

void sndMgrPlaySfxLooped( const char* pFilePath, int loopCount )
{
    if ( sndMgrIsSfxPlaying( pFilePath ) )
    {
        sndMgrLog( "SFX already playing: %s", pFilePath );
        return;
    }

    sndMgrLog( "Play SFX: %s", pFilePath );

    s32 channel = sndMgrFindNextAvailableChannel();
    dbgAssertMsg( channel != -1, "Out of channels" );

    Mix_Chunk* pChunk = utilDictLookup( &spChunkCache, pFilePath, strlen( pFilePath ) );
    if ( !pChunk )
    {
        pChunk = Mix_LoadWAV( pFilePath );
        utilDictAdd( &spChunkCache, pChunk, sizeof( Mix_Chunk ) );
    }

    spChunks[ channel ].pData = pChunk;
    spChunks[ channel ].pFilePath = pFilePath;

    Mix_PlayChannel( channel, pChunk, loopCount );
}