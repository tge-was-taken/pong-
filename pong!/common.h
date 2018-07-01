#pragma once

// Standard library
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

// Primitive types
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int64_t  s64;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;
typedef enum { false, true } bool;

// Convienience macros
#define utilMacroStatement( BODY ) do { BODY } while ( false )
#define utilUnused( VARIABLE ) ((void)VARIABLE)

// Our library
#include "dbg.h"
#include "mem.h"