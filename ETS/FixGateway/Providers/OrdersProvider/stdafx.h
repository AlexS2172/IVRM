// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

using namespace ATL;

#pragma warning(disable: 4503)

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


#pragma warning(push)
#pragma warning(disable : 4244 4267)
#include <QuickFix\Application.h>
#include <QuickFix\MessageCracker.h>
#include <QuickFix\FileStore.h>
#include <QuickFix\FileLog.h>
#include <QuickFix\SocketInitiator.h>
#include <QuickFix\SessionSettings.h>
#include <QuickFix\Dictionary.h>
#include <QuickFix\SessionID.h>
#include <QuickFix\Exceptions.h>
#include <QuickFix\Session.h>
#include <QuickFix\Utility.h>
#include <QuickFix\Mutex.h>
#include <QuickFix\Messages.h>
#pragma warning(pop)

#pragma comment(lib, "quickfix.lib")

#include <Boost/shared_ptr.hpp>

#pragma warning(disable:4192)

#import  "../../../tlb/ETSXMLParams.tlb" no_namespace named_guids
#import  "../../../tlb/FixProviders.tlb" no_namespace named_guids

#include "../../../ETSXMLParams\Helper\XMLParamsHelper.h"

#define OCON_PROVIDER_ID    0L
#define OCON_PROVIDER_NAME  L"OConnor Fix Provider"

#define FIX_VERSION			42L

#include <float.h>
#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647
