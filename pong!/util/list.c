
#include "list.h"

void _utilListInit( utilList* pList, size_t initialCapacity, size_t elementSize, bool userAllocated )
{
    pList->Capacity = initialCapacity;
    pList->ElementSize = elementSize;
    pList->pData = memAlloc( initialCapacity * elementSize );
    pList->Count = 0;
    pList->_UserAllocated = userAllocated;
}

utilList* utilListCreate( size_t initialCapacity, size_t elementSize )
{
    utilList* pList = memAlloc( sizeof( utilList ) );
    _utilListInit( pList, initialCapacity, elementSize, false );
    return pList;
}

void utilListInit( utilList* pList, size_t initialCapacity, size_t elementSize )
{
    _utilListInit( pList, initialCapacity, elementSize, true );
}

void utilListAdd( utilList* pList, void* pData )
{
    void* pDest = (void*)( (size_t)pList->pData + ( pList->Count * pList->ElementSize ) );

    pList->Count++;

    if ( pList->Count > pList->Capacity )
    {
        pList->Capacity *= 2;
        pList->pData = memReAlloc( pList->pData, pList->Capacity * pList->ElementSize );
    }

    // Copy element into list
    memcpy( pDest, pData, pList->ElementSize );
}

void utilListDestroy( utilList* pList )
{
    memFree( pList->pData );

    if ( !pList->_UserAllocated )
        memFree( pList );
}

void utilListSort( utilList* pList, utilListElementCompareFunc compareFunc )
{
    qsort( pList->pData, pList->Count, pList->ElementSize, compareFunc );
}