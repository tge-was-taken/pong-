#pragma once

#include "common.h"

#define dbgLogBase( MODULE, FORMAT, ... )                  printf( #MODULE ":\t\t " FORMAT "\n", ##__VA_ARGS__)
#define dbgErrorBase( MODULE, FORMAT, ... )                printf( #MODULE ":\t\t ERROR in %s (%s:%d): " FORMAT "\n", __FUNCTION__ "()", __FILE__, __LINE__, ##__VA_ARGS__ )
#define dbgAssertMsgBase( MODULE, CONDITION, FORMAT, ... ) do { if(!(CONDITION)) { dbgErrorBase( MODULE, FORMAT, ##__VA_ARGS__ ); assert( CONDITION ); } } while ( false )

#define dbgLog( FORMAT, ... )                     dbgLogBase( dbg, FORMAT, ##__VA_ARGS__ )    
#define dbgError( FORMAT, ... )                   dbgErrorBase( dbg, FORMAT, ##__VA_ARGS__ )
#define dbgAssertMsg( CONDITION, FORMAT, ... )    dbgAssertMsgBase( dbg, CONDITION, FORMAT, ##__VA_ARGS__ )
#define dbgAssert( CONDITION )                    dbgAssertMsg( CONDITION, "Assertion failed: " #CONDITION )