 // stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B6CC6C3B_6AAB_4833_96D8_0BB938D78768__INCLUDED_)
#define AFX_STDAFX_H__B6CC6C3B_6AAB_4833_96D8_0BB938D78768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable:4786)
#pragma warning(disable:4192)

#include <afxinet.h>
#include <afxdisp.h>
#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
#include <atlconv.h>

#import "../../../tlb/msgstruct.tlb" no_namespace named_guids
#import "../../../tlb/mmw.tlb" no_namespace named_guids
#import "../../../tlb/etsmanager.tlb" no_namespace named_guids
#import "../../../tlb/Oledb32.tlb" rename_namespace("Oledb32") named_guids

#import "../../../tlb/ETSXMLParams.tlb" no_namespace named_guids
#include "XMLParamsHelper.h"

#pragma warning(disable : 4786)

#define EG_DB_BACKWARD_COMPATIBLE

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibDB.h>
#include <EgLib/EgLibReg.h>
#include <EgStdLib/EgStdLib7.h>
#include <EgStdLib/Trace.h>
//#include <EgStdLib/Thread.h>

using namespace EgStd;

#include <set>
#include <math.h>

#define SETTINGS_XML_KEY		 _T("ETS\\CIBCTradeFeedAdapter")

//#define SETTINGS_REGISTRY_KEY	 _T("Software\\Egar\\ETS\\CIBCTradeFeedAdapter")
#define SETTINGS_TRADES_COUNT	 _T("Trades_Count")
#define SETTINGS_SLEEP_TIME		 _T("Waiting_Time")
#define SETTINGS_PUBLISH_DELAY	 _T("Publish_Delay")
#define SETTINGS_DB_SOURCE		 _T("Exchange_DB")

#define MODE_INTRADAY  0 
#define MODE_ENDOFADAY 1

#define MIN_TRADES_COUNT 10
#define MAX_TRADES_COUNT 1000

#define MIN_WAITING_TIME 1000
#define DEFAULT_WAITING_TIME 5000
#define MAX_WAITING_TIME 20000

#define MIN_PUBLISH_DELAY 0
#define MAX_PUBLISH_DELAY 10000

using namespace EgLib;
using namespace std;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B6CC6C3B_6AAB_4833_96D8_0BB938D78768__INCLUDED_)
