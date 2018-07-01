#pragma once

#include "common.h"

// Initialize the memory manager.
void memMgrInit();

// Destroy the memory manager.
void memMgrDestroy();

// Allocates memory and zeroes it.
void* memZeroAlloc( size_t size );

// Frees previously allocated memory.
void memFree( void* pMemory );

// Zeroes a memory block.
void memZero( void* pMemory, size_t size );

// Re-allocates a block of memory, if possible. Otherwise it allocates a new block.
void* memReAlloc( void* pMemory, size_t size );

void* memCAlloc( size_t num, size_t size );

void* _memDbgAlloc( size_t size, const char* pFile, const char* pFunction, u32 line );
void* _memAlloc( size_t size );

void memMgrEnterSection( const char* pName );
void memMgrExitSection();

#if _DEBUG
#define memAlloc( size ) _memDbgAlloc( size, __FILE__, __FUNCTION__, __LINE__ )
#else
#define memAlloc( size ) _memAlloc( size )
#endif