#include "WarningsOff.h"

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <eh.h>

// STL will not compile at /W4 /WX.  Not good.
#ifndef _DEBUG
// function '' not inlined
#pragma warning ( disable : 4710 )
#endif
#pragma warning( push, 3 )
#include <vector>
using namespace std ;
#pragma warning( pop )

#include "WarningsOn.h"


