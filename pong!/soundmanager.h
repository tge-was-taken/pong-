#pragma once

#include "common.h"

bool sndMgrInit();
void sndMgrDestroy();
void sndMgrPlayMusic( const char* pFilePath );
void sndMgrPauseMusic();
void sndMgrResumeMusic();
void sndMgrStopMusic();
bool sndMgrIsPlayingMusic();
void sndMgrPlaySfx( const char* pFilePath );
void sndMgrPlaySfxLooped( const char* pFilePath, int loopCount );