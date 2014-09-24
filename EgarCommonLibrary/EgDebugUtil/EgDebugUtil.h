#ifndef _EGDEBUGUTIL_H
#define _EGDEBUGUTIL_H

/*//////////////////////////////////////////////////////////////////////
                                Includes
//////////////////////////////////////////////////////////////////////*/
// If windows.h has not yet been included, include it now.
#ifndef _INC_WINDOWS
#include <windows.h>
#endif

#include <imagehlp.h>

/*//////////////////////////////////////////////////////////////////////
                            Special Defines
//////////////////////////////////////////////////////////////////////*/
// The defines that set up how the functions or classes are exported or
//  imported.
#ifndef EGDEBGUTIL_DLLINTERFACE
#ifdef BUILDING_EGEXEPTIONHANDLE_DLL
#define EGDEBGUTIL_DLLINTERFACE __declspec ( dllexport )
#else
#define EGDEBGUTIL_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_EGEXEPTIONHANDLE_DLL
#endif  // EGDEBGUTIL_DLLINTERFACE

/*//////////////////////////////////////////////////////////////////////
                            Special Includes
//////////////////////////////////////////////////////////////////////*/

// The minidump routines.
#include "MiniDump.h"

#endif  // _EGDEBUGUTIL_H


