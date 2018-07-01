#pragma once

#include "../common.h"

#define MATH_DIRECTION_NONE     0
#define MATH_DIRECTION_LEFT     -1
#define MATH_DIRECTION_RIGHT    1
#define MATH_DIRECTION_UP       -1
#define MATH_DIRECTION_DOWN     1

typedef struct
{
    f32 X;
    f32 Y;
} mathVec2;

inline void mathVec2Add( mathVec2* pThis, mathVec2* pOther )
{
    pThis->X += pOther->X;
    pThis->Y += pOther->Y;
}

inline void mathVec2Sub( mathVec2* pThis, mathVec2* pOther )
{
    pThis->X -= pOther->X;
    pThis->Y -= pOther->Y;
}

inline void mathVec2Mul( mathVec2* pThis, mathVec2* pOther )
{
    pThis->X *= pOther->X;
    pThis->Y *= pOther->Y;
}

inline void mathVec2Inv( mathVec2* pVec )
{
    pVec->X = -pVec->X;
    pVec->Y = -pVec->Y;
}

#define mathMax( X, Y ) ( (X) > (Y) ? (X) : (Y) )
#define mathMin( X, Y ) ( (X) < (Y) ? (X) : (Y) )
#define mathClamp( VALUE, MIN, MAX ) ( (VALUE) < (MIN) ? (MIN) : (VALUE) > (MAX) ? (MAX) : (VALUE) )
#define mathIsAxisInRange( AXIS, EXTENT, MIN, MAX ) ( (AXIS) >= (MIN) && ( (AXIS) + (EXTENT) ) <= (MAX) )
#define mathIsValueInRange( VALUE, MIN, MAX ) ( (VALUE) >= (MIN) ) && ( (VALUE) <= (MAX) )