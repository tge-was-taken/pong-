#pragma once

#include "../common.h"
#include "math.h"
#include "time.h"

extern u32 gRandSeed;

// Set random seed.
inline void utilRandSetSeed( u32 seed )
{
    srand( gRandSeed = seed );
}

// Initialize random seed
inline void utilRandInit()
{
    dbgLogBase( utilRand, "initializing" );
    utilRandSetSeed( (u32)time( NULL ) );
}

// Generate next random number.
inline s32 utilRandNext()
{
    return rand();
}

// Generate random number between min and max, inclusive.
inline s32 utilRandBetween( s32 min, s32 max )
{
    return ( rand() % max ) + min;
}

// Generate random chance specified by the percent.
inline bool utilRandPercent( s32 percent )
{
    return utilRandBetween( 0, 100 ) <= percent;
}

// Generate random boolean
inline bool utilRandBool()
{
    return ( bool )utilRandBetween( 0, 1 );
}