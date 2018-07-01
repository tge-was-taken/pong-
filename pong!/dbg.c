
#include "dbg.h"
#include "win.h"

void dbgInit()
{
    // Create console
    AllocConsole();

    // Redirect standard text outputs to the console
    freopen_s( (FILE**)stdout, "CONOUT$", "w", stdout );
    freopen_s( (FILE**)stderr, "CONOUT$", "w", stderr );

    dbgLog( "initialized" );
}