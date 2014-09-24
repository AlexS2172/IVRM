// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
//#define _ATL_DEBUG_INTERFACES

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <comdef.h>

using namespace ATL;

#pragma warning( disable : 4290 )
#include <ComErrorWrapper.h>

// ADO stuff
#define EG_DB_BACKWARD_COMPATIBLE
#include <EgLib/egLibDbg.h>
#include <EgLib/egLibSync.h>
#include <EgLib/egLibThread.h>
#include <EgLib/egLibMisc.h>
#include <EgLib/egLibReg.h>
#include <EgLib/egLibDB.h>

//#include <csWrapper.h>
#include <DoubleBuffer.h>

using namespace EgLib;

/*
#import "progid:VMMessaging.VMTransport" raw_interfaces_only named_guids no_namespace //embedded_idl
#import "..\tlb\VMStructures.tlb" rename_namespace("VmStruct")
#import "..\tlb\VMCurve.tlb" rename_namespace("VmCurve")

*/
// VA DB Layout
#import "..\tlb\VADBLayout.tlb" no_namespace, named_guids


// Messages structure
#import "..\tlb\MsgStruct.tlb" no_namespace, named_guids
//#import "..\tlb\MMW.tlb" no_namespace, named_guids
//#import "..\tlb\VME.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search
//#import "..\tlb\ETSAdapter.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search, exclude("_IVolatilitySourceEvents")
class CVSDataStorage;
extern CVSDataStorage g_VSDataStorage;