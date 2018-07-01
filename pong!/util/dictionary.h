#pragma once

#include "../common.h"
#include "list.h"

typedef struct
{
    utilList List;
    bool _UserAllocated;
} utilDict;

typedef struct
{
    u32 Key;
    void* pValue;
} utilDictKvp;

void utilDictInit( utilDict* pDict, size_t initialCapacity );
utilDict* utilDictCreate( size_t initialCapacity );
void utilDictAdd( utilDict* pDict, void* pData, size_t size );
void* utilDictLookup( utilDict* pDict, void* pKey, size_t size );
void utilDictDestroy( utilDict* pDict );