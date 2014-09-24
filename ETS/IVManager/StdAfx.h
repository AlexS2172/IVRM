// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__83E28028_6A10_416C_92B9_0A385A3F4193__INCLUDED_)
#define AFX_STDAFX_H__83E28028_6A10_416C_92B9_0A385A3F4193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500

#include <math.h>
#include <float.h>

#pragma warning(disable: 4786)

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>

#include <afxwin.h>         // MFC core and standard components
#include <afxole.h>
#include <afxinet.h>
#include <comdef.h>
#include <comip.h>

#pragma comment(lib, "wininet.lib")

#define _ATL_APARTMENT_THREADED


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CIVAnalyzerModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CIVAnalyzerModule _Module;
#include <atlcom.h>

#ifdef _IVAN_DB_ACCESS
#define IGNORE_MSSQL_SPECIFIC 
#endif

#pragma warning(disable: 4786)

#define EG_DB_BACKWARD_COMPATIBLE

#include <EgLib/egLibDbg.h>
#include <EgLib/egLibSync.h>
#include <EgLib/egLibThread.h>
#include <EgLib/egLibMisc.h>
#include <EgLib/egLibReg.h>
#include <EgLib/egLibDB.h>
//using namespace EgLib;

//#include <csWrapper.h>
#include <OLEDBERR.h>


#ifndef _MULTIPROVIDERS
	#ifndef _VOLA_BUILDER
		#import "..\tlb\IVMCtrl.tlb" rename_namespace ("IVM")
		#import "MyTrackProviders.tlb" rename_namespace ("MT")
		#import "WorkFlowIC.dll" rename_namespace ("IC")  // for about info
	#else
		#import "..\tlb\IVMCtrl.tlb" rename_namespace ("IVM")
		#import "MyTrackDataProviders.tlb" rename_namespace ("MT")
		#import "WorkFlowIC.dll" rename_namespace ("IC")  // for about info
	#endif
#else
	#ifndef _VOLA_BUILDER
		//#import "MyTrackProviders.tlb" rename_namespace ("MT")
		#import "PriceProviders.tlb" rename_namespace ("PP")
		#import "..\tlb\IVMCtrl.tlb" rename_namespace ("IVM")
		#import "..\bin\WorkFlowIC.dll" rename_namespace ("IC")  // for about info
	#else
		#import "..\tlb\IVMCtrl.tlb" rename_namespace ("IVM")
		//#import "MyTrackProviders.tlb" rename_namespace ("MT")
		#import "PriceProviders.tlb" rename_namespace ("PP")
		#import "WorkFlowIC.dll" rename_namespace ("IC")  // for about info
	#endif
#endif

#ifdef _VOLA_MANAGER
	#import "..\tlb\VADBLayout.tlb" no_namespace named_guids
	#import "..\tlb\MsgStruct.tlb"  no_namespace named_guids
	#import "..\tlb\MMW.tlb"  rename_namespace("MMW") named_guids
	#import "..\tlb\VME.tlb" rename_namespace("VME") named_guids
	#import "..\tlb\ETSManager.tlb"  rename_namespace("Stg")
	
	#import "..\Components\tlb\EtsGeneral.tlb"  no_namespace 
#endif //_VOLA_MANAGER

#import "../tlb/ETSXMLParams.tlb" no_namespace named_guids

#ifndef _VOLA_BUILDER
#include  <OptionCalc\OptionCalc.h>
#else
#include  <OptionCalc\OptionCalc.h>
#endif

#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>
#include "..\include\EGOcx.h"

#include <oledb.h>
#include <MSDasc.h> 

#include "..\edaTools\edaTools.h"

#include "MonitorFunctionsWrapper.h"

#include "..\..\EgarCommonLibrary\Include\Eglib\EgLibComError.h"
using namespace EgLib;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__83E28028_6A10_416C_92B9_0A385A3F4193__INCLUDED_)
