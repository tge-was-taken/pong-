#pragma once

#include "../common.h"

typedef s32 ( *utilListElementCompareFunc )( const void* pA, const void* pB );

typedef struct
{
    void* pData;
    size_t Count;
    size_t Capacity;
    size_t ElementSize;
    bool _UserAllocated;
} utilList;

utilList* utilListCreate( size_t initialCapacity, size_t elementSize );
void utilListInit( utilList* pList, size_t initialCapacity, size_t elementSize );
void utilListAdd( utilList* pList, void* pData );
void utilListDestroy( utilList* pList );
void utilListSort( utilList* pList, utilListElementCompareFunc compareFunc );