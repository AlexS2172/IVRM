#ifndef __STD_AFX_H__
#define __STD_AFX_H__

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define _ATL_FREE_THREADED

#include <afxcmn.h>
#include <afxwin.h>		// MFC core and standard components
#include <afxext.h>		// MFC extensions
#include <afxdisp.h>	// MFC Automation extensions 

/***********************************************************
					ATL Support 
***********************************************************/
#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
};

extern CExeModule _Module;

#include <atlcom.h>

#include <comdef.h>

/***********************************************************
					ATL Support 
***********************************************************/

/***********************************************************
					STL Support 
***********************************************************/
#pragma warning(disable: 4503)

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
using namespace std;

#include "Constants.h"	
#include "UICmdDefs.h"
#include <process.h>
#include <winsock2.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
using namespace EgLib;


#import "..\tlb\Transport.tlb" no_namespace named_guids raw_interfaces_only rename("SendMessage", "SendMessageX")
#import "..\tlb\EgTpPrx.tlb" no_namespace named_guids raw_interfaces_only

#endif // !defined(__STD_AFX_H__)
