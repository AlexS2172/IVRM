// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5802C809_674D_4708_96DF_434213BB6ADC__INCLUDED_)
#define AFX_STDAFX_H__5802C809_674D_4708_96DF_434213BB6ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#define _WIN32_WINNT 0x0400
// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
//#include <objbase.h>


#include <atlbase.h>

extern CComModule _Module;

#include <atlcom.h>
#include <comdef.h>

#import "..\tlb\SimParams.tlb" rename_namespace("SP")


using namespace std;
#include <vector>
#include <queue>
#include <map>

#define IGNORE_MSSQL_SPECIFIC
#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibException.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibDb.h>
using namespace EgLib;

#include <boost/smart_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

// TODO: reference additional headers your program requires here
extern "C" {
#include "rt_API.h" 
#include "rt_SIM.h"
}

extern  CRt_Sim g_RT;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5802C809_674D_4708_96DF_434213BB6ADC__INCLUDED_)
