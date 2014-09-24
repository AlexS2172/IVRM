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
#include <atlutil.h>

using namespace ATL;

// Please add "EgCommon\include" path to "Tools->Options->VC++ Directories->Include Files" section
#define EG_DB_BACKWARD_COMPATIBLE
#include <EgLib/egLibDbg.h>
#include <EgLib/egLibSync.h>
#include <EgLib/egLibThread.h>
#include <EgLib/egLibMisc.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/egLibReg.h>
#include <EgLib/egLibDB.h>
#include <EgLib/EgLibTrace.h>
using namespace EgLib;

#include <EgLib\EgLibComError.h>
#include <vcue\collections.h>
#include <ComponentsCommon.h>
#include <EtsDate.h>
#include <Boost/shared_array.hpp>
#include <Boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "threadpool.h"
#include "GenegalSettings.h"

#include <Record.h>
#import "..\..\..\MarketDataAdapters\tlb\PriceProviders.tlb" rename("SettlementTypeEnum", "PpSettlementTypeEnum") no_namespace named_guids
#import "..\..\..\MarketDataAdapters\DataFeedProviders\Release\DataFeedProviders.tlb" no_namespace named_guids

#include <OptionCalc\OptionCalc.h>
#include <MarketCommon.h>
#include "IvRmException.h"
#include <hash_map>

using namespace IvRmCore;

#import "..\..\tlb\MsgStruct.tlb"			no_namespace named_guids
#import "..\..\tlb\VADBLayout.tlb"		no_namespace named_guids
#import "..\..\tlb\VolatilitySources.tlb"	no_namespace named_guids

inline double round(double dblValue) 
{
	return floor(dblValue + .5);
}

//-------------------------------------------------------------------------------------------------------------//
#define TRACE_COM_ERROR(__error)																			\
	EgLib::CEgLibTraceManager::Trace(LogError,																\
	__FUNCTION__,															\
	_TEXT("ComError: %s[%d] FileName: %s Line: %d"),						\
	(LPCSTR)__error.Description(), __error.Error(), __FILE__, __LINE__)		\

#define TRACE_WARNING(__format, __description)																	\
	EgLib::CEgLibTraceManager::Trace(LogWarning,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_INFO(__format, __description)																	\
	EgLib::CEgLibTraceManager::Trace(LogInfo,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_DEBUG(__format, __description)																\
	EgLib::CEgLibTraceManager::Trace(LogDebug,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_SYSTEM(__format, __description)																\
	EgLib::CEgLibTraceManager::Trace(LogSystem,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_ERROR(__format, __description) 																\
	EgLib::CEgLibTraceManager::Trace(LogError,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_UNKNOWN_ERROR() 																				\
	EgLib::CEgLibTraceManager::Trace(LogError,																\
	__FUNCTION__,																			\
	_TEXT("Unknown error! %s[%d] WinError: [%d]"),											\
	__FILE__, __LINE__, ::GetLastError())														\

#define TRACE_COM_ERROR_EX(__error, __info, __description)												\
	EgLib::CEgLibTraceManager::Trace(LogError,															\
	__FUNCTION__,																						\
	__info _TEXT("ComError: %s[%d] FileName: %s Line: %d"),												\
	__description,(LPCSTR)__error.Description(), __error.Error(), __FILE__, __LINE__)					\
//-------------------------------------------------------------------------------------------------------------//
