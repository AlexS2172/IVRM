//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	StdAfx.h
// created:		27-Dec-2002 17:10 by Suchkov Dmitry
//
// purpose:		include file for standard system include files,
//				or project specific include files that are used frequently, but
//				are changed infrequently
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define VC_EXTRALEAN

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

//**************************************************************************************************************//
// libaries
//**************************************************************************************************************//
#pragma comment( lib, "shlwapi.lib" )
#pragma comment( lib, "wininet.lib" )

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
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

#include <process.h>
#include <time.h>

#include <string>
#include <set>
#include <queue>
#include <fstream>

#include "winsock.h"

#include <comdef.h>

#define EG_DB_BACKWARD_COMPATIBLE
#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibDB.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibStrategy.h>
#include <Boost/lexical_cast.hpp>

#include <Win32Error.h>

#include "Tracer.h"
#include "ModulVer.h"

#pragma warning(disable:4192 4786)

#import "../../../tlb/msgstruct.tlb" no_namespace named_guids
#import "../../../tlb/mmw.tlb" no_namespace named_guids
#import "../../../tlb/etsmanager.tlb" no_namespace named_guids
#import "../../../tlb/Oledb32.tlb" rename_namespace("Oledb32") named_guids

#import "../../../tlb/ETSXMLParams.tlb" no_namespace named_guids
#include "XMLParamsHelper.h"

#include <egdebugutil.h>
//**************************************************************************************************************//
// variables
//**************************************************************************************************************//
extern const int MonthDays[];
extern LPCSTR g_cMonthName[];

//**************************************************************************************************************//
// using directives & declarations
//**************************************************************************************************************//
using namespace std;
using namespace EgLib;

//{{AFX_INSERT_LOCATION}}