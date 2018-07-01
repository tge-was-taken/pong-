#pragma once

#include "../common.h"

typedef void ( *utilLinkedListEntryDataDestroyFunc )( void* pData );
typedef bool ( *utilLinkedListPredicateFunc )( struct utilLinkedListEntry* pEntry, void* pParams );
typedef void ( *utilLinkedListActionFunc )( struct utilLinkedListEntry* pEntry, void* pParams );

typedef struct
{
    void* pData;
    utilLinkedListEntryDataDestroyFunc Destroy;
    struct utilLinkedListEntry* pNext;
} utilLinkedListEntry;

typedef struct
{
    utilLinkedListEntry* Head;
    utilLinkedListEntry* Tail;
    bool _UserAllocated;
} utilLinkedList;

utilLinkedList* utilLinkedListCreate();
void utilLinkedListInit( utilLinkedList* pList );

// Add entry to the list.
utilLinkedListEntry* utilLinkedListAdd( utilLinkedList* pList, void* pData, utilLinkedListEntryDataDestroyFunc destroy );

// Remove entry from the list.
void utilLinkedListRemove( utilLinkedList* pList, utilLinkedListEntry* pEntryToRemove );

// Remove entry from the list using a predicate.
void utilLinkedListRemovePred( utilLinkedList* pList, utilLinkedListPredicateFunc pred, void* pParams );

// Iterate over all entries and perform an action on each one.
void utilLinkedListForEach( utilLinkedList* pList, utilLinkedListActionFunc action, void* pParams );

// Frees the memory allocated for each entry in the list
void utilLinkedListDestroy( utilLinkedList* pList );