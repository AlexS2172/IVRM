// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

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

#define _ATL_FREE_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#pragma warning(push)
#pragma warning(disable: 4290)

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
using namespace ATL;

#include <string>
using namespace std;

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibDynamicGITArray.h>
using namespace EgLib;

#include <Boost/shared_ptr.hpp>

#import  "..\tlb\ETSXMLParams.tlb" no_namespace named_guids
#include "..\ETSXMLParams\Helper\XMLParamsHelper.h"

/////////////////////////////////////////////////////////////////////////////
#include <float.h>
#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647
/////////////////////////////////////////////////////////////////////////////
//
static __inline bool IsBadValue(long nValue)
{
	return (BAD_LONG_VALUE == nValue);
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline bool IsBadValue(double dValue)
{
	return !(_finite(dValue) && dValue > BAD_DOUBLE_VALUE);
}


#include "FIXGGeneral.h"

#include <winsock2.h>

const LPCTSTR REG_KEY	= _T("SOFTWARE\\EGAR\\ETS\\FIXGateway\\Settings");

#pragma warning(pop)
