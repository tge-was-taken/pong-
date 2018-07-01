
#include "mem.h"

#include <SDL.h>

#define memMgrLog(FORMAT, ...)     dbgLogBase( memMgr, FORMAT, ##__VA_ARGS__ )
#define memMgrError(FORMAT, ...)   dbgErrorBase( memMgr, FORMAT, ##__VA_ARGS__ )

typedef struct
{
    size_t Size;
    const void* pMem;
    const char* pFileName;
    const char* pFunctionName;
    u32 Line;
} memDbgAllocInfo;

typedef struct
{
    memDbgAllocInfo Info;
    struct memDbgAllocInfoListEntry* pNext;
} memDbgAllocInfoListEntry;

static memDbgAllocInfoListEntry* spDbgAllocInfoListHead = NULL;
static memDbgAllocInfoListEntry* spDbgAllocInfoListTail = NULL;
static const char* spSectionName = NULL;

// Allocate and create an allocation info list entry.
static memDbgAllocInfoListEntry* memDbgCreateListEntry( size_t size, const void* pMem, const char* pFile, const char* pFunction, u32 line )
{
    memDbgAllocInfoListEntry* pEntry = _memAlloc( sizeof( memDbgAllocInfoListEntry ) );
    pEntry->Info.Size = size;
    pEntry->Info.pMem = pMem;
    pEntry->Info.pFileName = pFile;
    pEntry->Info.pFunctionName = pFunction;
    pEntry->Info.Line = line;
    pEntry->pNext = NULL;
    return pEntry;
}

// Add an allocation entry to the allocation info list.
static void memDbgAddAllocInfo( size_t size, const void* pMem, const char* pFile, const char* pFunction, u32 line )
{
    memDbgAllocInfoListEntry* pEntry = memDbgCreateListEntry( size, pMem, pFile, pFunction, line );

    memMgrLog( "Allocated %08X bytes at %08X from %s:%s:%d", (u32)size, (u32)pMem, pFile, pFunction, line );

    if ( spDbgAllocInfoListTail == NULL )
    {
        spDbgAllocInfoListTail = spDbgAllocInfoListHead = pEntry;
    }
    else
    {
        spDbgAllocInfoListHead->pNext = pEntry;
        spDbgAllocInfoListHead = pEntry;
    }
}

// Update alloc info, for realloc
static void memDbgUpdateAllocInfo( void* pMem, void* pNewMem, size_t size )
{
    memDbgAllocInfoListEntry* pEntry = spDbgAllocInfoListTail;
    memDbgAllocInfoListEntry* pPrevEntry = NULL;

    // Find allocation with this address
    while ( pEntry != NULL )
    {
        if ( pEntry->Info.pMem == pMem )
        {
            pEntry->Info.pMem = pNewMem;
            pEntry->Info.Size = size;
        }

        pPrevEntry = pEntry;
        pEntry = pEntry->pNext;
    }
}

static void memSDLAlloc( size_t size )
{
#if _DEBUG
    _memDbgAlloc( size, NULL, spSectionName, -1 );
#else
    _memAlloc( size );
#endif
}

// Remove an allocation from the allocation info list using the address that was allocated.
static void memDbgRemoveAllocInfo( void* pMem )
{
    memDbgAllocInfoListEntry* pEntry = spDbgAllocInfoListTail;
    memDbgAllocInfoListEntry* pPrevEntry = NULL;

    // Find allocation with this address
    while ( pEntry != NULL )
    {
        if ( pEntry->Info.pMem == pMem )
        {
            if ( pPrevEntry )
            {
                pPrevEntry->pNext = pEntry->pNext;
            }
            else
            {
                // pPrevEntry can only be null if it is the tail.
                spDbgAllocInfoListTail = pEntry->pNext;
            }

            memMgrLog( "Freeing %08X bytes at %08X from %s:%s:%d", (u32)pEntry->Info.Size, (u32)pMem, pEntry->Info.pFileName, pEntry->Info.pFunctionName, pEntry->Info.Line );
            free( pEntry );
            break;
        }

        pPrevEntry = pEntry;
        pEntry = pEntry->pNext;
    }
}

void memMgrInit()
{
    memMgrLog( "initializing" );

#ifdef DBG_MEM_SDL
    SDL_SetMemoryFunctions( memSDLAlloc, memCAlloc, memReAlloc, memFree );
#endif
}

void memMgrDestroy()
{
    memMgrLog( "destroying" );

    memDbgAllocInfoListEntry* pEntry = spDbgAllocInfoListTail;

    // Find allocation with this address
    while ( pEntry != NULL )
    {
        memDbgAllocInfoListEntry* pNextEntry = pEntry->pNext;

        dbgAssertMsg( false, "Leaked %08X bytes at %08X from %s:%s:%d", (u32)pEntry->Info.Size, (u32)pEntry->Info.pMem, pEntry->Info.pFileName, pEntry->Info.pFunctionName, pEntry->Info.Line );

        free( pEntry );
        pEntry = pNextEntry;
    }
}

void* _memAlloc( size_t size )
{
    void* pMemory = malloc( size );
    dbgAssertMsg( pMemory != NULL, "mem: Failed to allocate %08X bytes of memory", (u32)size );
    return pMemory;
}

void* _memDbgAlloc( size_t size, const char* pFile, const char* pFunction, u32 line )
{
    void* pMem = _memAlloc( size );
    memDbgAddAllocInfo( size, pMem, pFile, pFunction, line );
    return pMem;
}

void* memZeroAlloc( size_t size )
{
    void* pMemory = memAlloc( size );
    memZero( pMemory, size );
    return pMemory;
}

void* memCAlloc( size_t num, size_t size )
{
    void* pMemory = memAlloc( num * size );
    memZero( pMemory, size );
    return pMemory;
}

void* memReAlloc( void* pMemory, size_t size )
{
    void* pNewMemory = realloc( pMemory, size );

#if _DEBUG
    memDbgUpdateAllocInfo( pMemory, pNewMemory, size );
#endif

    dbgAssertMsg( pNewMemory != NULL, "mem: Failed to re-allocate %08X bytes of memory", (u32)size );
    return pNewMemory;
}

void memFree( void* pMem )
{
#if _DEBUG
    memDbgRemoveAllocInfo( pMem );
#endif

    free( pMem );
}

void memZero( void* pMemory, size_t size )
{
    memset( pMemory, 0, size );
}

void memMgrEnterSection( const char* pName )
{
#ifdef _DEBUG
    spSectionName = pName;
#else
    utilUnused( pName );
#endif
}

void memMgrExitSection()
{
    spSectionName = NULL;
}