// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0500	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#define _ATL_DEBUG_INTERFACES

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <sqlext.h>
#include <afxctl.h>
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")

#include <shlwapi.h>
#include <comdef.h>
#include <process.h>

#include <ComErrorWrapper.h>
//#include <Lock.h>

#import "msado25.tlb"		no_namespace named_guids rename("EOF", "AdoEof") rename("BOF", "AdoBof")
#import "..\..\EgarTransport\tlb\Transport.tlb" no_namespace named_guids rename("SendMessage", "SendTransportMessage")
#import "..\..\EgarTransport\tlb\EgTpPrx.tlb" no_namespace named_guids
#import "..\tlb\MsgStruct.tlb" no_namespace named_guids
#import "..\tlb\MMW.tlb" no_namespace named_guids
#import "..\tlb\ETSManager.tlb" rename_namespace("EM") named_guids
#import "..\tlb\FixProviders.tlb"  rename_namespace("FP") named_guids
//#import "FIXOCON.tlb" no_namespace named_guids
#import "..\tlb\FIXGateway.tlb" no_namespace named_guids
#import <msdatsrc.tlb> no_namespace named_guids
#import <vsflex7.ocx> no_namespace named_guids

#include <vector>
#include <map>
#include <atlbase.h>
#include <atlcom.h>
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

const LPCTSTR REG_KEY	= _T("SOFTWARE\\EGAR");
const LPCTSTR REG_GATEWAY_KEY	= _T("SOFTWARE\\EGAR\\ETS\\FIXGateway\\Settings");
const LPCTSTR REG_AUTOSTART_KEY	= _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
