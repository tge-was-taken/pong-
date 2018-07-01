
#include "linkedlist.h"

static void _utilLinkedListInit( utilLinkedList* pList, bool userAllocated )
{
    pList->_UserAllocated = userAllocated;
    pList->Head = NULL;
    pList->Tail = NULL;
}

void utilLinkedListInit( utilLinkedList* pList )
{
    _utilLinkedListInit( pList, true );
}

utilLinkedList* utilLinkedListCreate()
{
    utilLinkedList* pList = (utilLinkedList*)memAlloc( sizeof( utilLinkedList ) );
    _utilLinkedListInit( pList, false );
    return pList;
}

utilLinkedListEntry* utilLinkedListAdd( utilLinkedList* pList, void* pData, utilLinkedListEntryDataDestroyFunc destroy )
{
    utilLinkedListEntry* pEntry = (utilLinkedListEntry*)memAlloc( sizeof( utilLinkedListEntry ) );
    pEntry->pData = pData;
    pEntry->pNext = NULL;
    pEntry->Destroy = destroy;

    if ( pList->Tail == NULL )
    {
        pList->Tail = pList->Head = pEntry;
    }
    else
    {
        pList->Head->pNext = pEntry;
        pList->Head = pEntry;
    }

    return pEntry;
}

#define utilLinkedListForEachDeclareBody( action ) \
    utilLinkedListEntry* pEntry = pList->Tail; \
    utilLinkedListEntry* pPrevEntry = NULL; \
    \
    while ( pEntry != NULL ) \
    { \
        utilLinkedListEntry* pNextEntry = pEntry->pNext; \
        action \
        pPrevEntry = pEntry; \
        pEntry = pNextEntry; \
    } \

void utilLinkedListForEach( utilLinkedList* pList, utilLinkedListActionFunc action, void* pParams )
{
    utilLinkedListForEachDeclareBody( action( pEntry, pParams ); )
}

#define utilLinkedListRemoveDeclareBody( cond ) \
    utilLinkedListForEachDeclareBody( \
    if ( cond ) \
        { \
            if ( pPrevEntry ) \
            { \
                pPrevEntry->pNext = pEntry->pNext; \
            } \
            else \
            { \
                pList->Tail = pEntry->pNext; \
            } \
            \
            memFree( pEntry ); \
            pEntry = NULL; \
            break; \
        } \
    ) \

void utilLinkedListRemove( utilLinkedList* pList, utilLinkedListEntry* pEntryToRemove )
{
    utilLinkedListRemoveDeclareBody( pEntry == pEntryToRemove )
}

void utilLinkedListRemovePred( utilLinkedList* pList, utilLinkedListPredicateFunc pred, void* pParams )
{
    utilLinkedListRemoveDeclareBody( pred( pEntry, pParams ) )
}

void utilLinkedListDestroy( utilLinkedList* pList )
{
    utilLinkedListForEachDeclareBody( if ( pEntry->Destroy ) pEntry->Destroy( pEntry->pData ); memFree( pEntry ); pEntry = NULL; );
        
    if ( !pList->_UserAllocated )
        memFree( pList );
}

#undef utilLinkedListRemoveDeclareBody
#undef utilLinkedListForEachDeclareBody