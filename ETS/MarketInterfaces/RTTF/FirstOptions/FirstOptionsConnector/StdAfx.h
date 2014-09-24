// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AA5422E0_BD4A_4ED5_905A_FF7F70438AD1__INCLUDED_)
#define AFX_STDAFX_H__AA5422E0_BD4A_4ED5_905A_FF7F70438AD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0400 

#pragma warning(disable: 4786)
#pragma warning(disable: 4503)

#include <stdio.h>
#include <windows.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <process.h>
#include <fstream>
#include <iostream>

#include <atlbase.h>

#include <XDR.h>
#include <Cstypes.h>
#include <CsMsgs.h>
#include <RttfMsgs.h>

#include "resource.h"

#include <comdef.h>
#include <eg_lib\egMisc.h>
#include <Win32Error.h>
#include <Lock.h>

#define EG_DB_BACKWARD_COMPATIBLE
#include <eg_lib\egDB.h>
#include <eg_lib\EgRegistry.h>

using namespace eg_lib;
using namespace eg_reg;

#define USE_SMART_LASTTRADETIME_ALGORITHM	
// FOC prog updates last trade time from trade after receiving signal that all
// trades are received by request
// by using USE_SMART_LASTTRADETIME_ALGORITHM we update last time choosing latest time
// from trades while receiving them. This allows us to set last time just at once with
// receiving the signal without waiting for next trade!
#define PROFILE_ON		0

#import "..\..\..\..\tlb\ETSManager.tlb" no_namespace named_guids 


using namespace std;

typedef basic_string<TCHAR>	tstring;

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AA5422E0_BD4A_4ED5_905A_FF7F70438AD1__INCLUDED_)
