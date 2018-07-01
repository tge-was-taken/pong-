
#include "dictionary.h"

static void _utilDictInit( utilDict* pDict, size_t initialCapacity, bool wasAllocated )
{
    utilListInit( &pDict->List, initialCapacity, sizeof( utilDictKvp ) );
    pDict->_UserAllocated = wasAllocated;
}

void utilDictInit( utilDict* pDict, size_t initialCapacity )
{
    _utilDictInit( pDict, initialCapacity, true );
}

utilDict* utilDictCreate( size_t initialCapacity )
{
    utilDict* pDict = memAlloc( sizeof( utilDict ) );
    _utilDictInit( pDict, initialCapacity, false );
    return pDict;
}

static u32 utilDictGenerateHash( void* pData, size_t size )
{
    u32 hash = 5381;
    u8* pCurData = (u8*)pData;

    for ( size_t i = 0; i < size; ++i )
    {
        hash = ( ( hash << 5 ) + hash ) + *pCurData++;
    }

    return hash;
}

static s32 utilDictListCompareFunc( const void* p1, const void* p2 )
{
    utilDictKvp* pA = (utilDictKvp*)p1;
    utilDictKvp* pB = (utilDictKvp*)p2;

    if ( pA->Key < pB->Key ) return -1;
    if ( pA->Key == pB->Key ) return 0;
    if ( pA->Key > pB->Key ) return 1;
}

void utilDictAdd( utilDict* pDict, void* pData, size_t size )
{
    utilDictKvp pKvp;
    pKvp.Key = utilDictGenerateHash( pData, size );
    pKvp.pValue = pData;

    utilListAdd( &pDict->List, &pKvp );
    utilListSort( &pDict->List, utilDictListCompareFunc );
}

void* utilDictLookup( utilDict* pDict, void* pKey, size_t size )
{
    if ( pDict->List.Count == 0 )
    {
        return NULL;
    }
    else if ( pDict->List.Count == 1 )
    {
        return ( *(utilDictKvp*)pDict->List.pData ).pValue;
    }

    u32 first = 0;
    u32 last = pDict->List.Count - 1;
    u32 middle = last / 2;
    u32 key = utilDictGenerateHash( pKey, size );

    while ( first <= last )
    {
        utilDictKvp* pKvp = &( (utilDictKvp*)pDict->List.pData )[ middle ];

        if ( pKvp->Key < key )
        {
            first = middle + 1;
        }
        else if ( pKvp->Key == key )
        {
            return pKvp->pValue;
        }
        else
        {
            last = middle - 1;
        }

        middle = ( first + last ) / 2;
    }

    return NULL;
}

void utilDictDestroy( utilDict* pDict )
{
    utilListDestroy( &pDict->List );

    if ( !pDict->_UserAllocated )
        memFree( pDict );
}