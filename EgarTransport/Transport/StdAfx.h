#ifndef __STD_AFX_H__
#define __STD_AFX_H__

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define _ATL_FREE_THREADED
#pragma warning(disable: 4503)
#include <winsock2.h>
#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CServiceModule : public CComModule
{
public:
	CServiceModule():CComModule()
	{
		 m_isRunning  = false;	
	}

	virtual ~CServiceModule()
	{
	}
	
	HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService);
	HRESULT UnregisterServer();
	void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid = NULL);
    void Start();
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void Handler(DWORD dwOpcode);
    void Run();
    BOOL IsInstalled();
    BOOL Install();
    BOOL Uninstall();
	LONG Lock();
	LONG Unlock();
    void SetServiceStatus(DWORD dwState);
    void SetupAsLocalServer();
	void Trace(long lLogLevel, const TCHAR* lpszCategory, const TCHAR* lpszMessage, ...);


//Implementation
private:
	bool m_isRunning;
	
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);

protected:

	HRESULT DoSettings();

// data members
public:
	
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;
	DWORD dwThreadID;
	BOOL m_bService;

};

extern CServiceModule _Module;
#include <atlcom.h>
#include <comdef.h>
#include <atlstr.h>
using namespace ATL;

#include <process.h>
#include <time.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
using namespace std;


#define BOOST_MEM_FN_ENABLE_STDCALL
#include <Boost\shared_ptr.hpp>
#include <boost\mem_fn.hpp>

#include <EgLib\EgLibDbg.h>
#include <EgLib\EgLibThread.h>
#include <EgLib\EgLibSync.h>
#include <EgLib\EgLibMisc.h>
#include <EgLib\EgLibreg.h>
#include <EgLib\EgLibStrategy.h>
#include <EgLib\EgLibTrace.h>
#include <EgLib\EgLibReg.h>
using namespace EgLib;


typedef basic_string < TCHAR, char_traits< TCHAR > , allocator< TCHAR > >	tstring;

#define _EGDEBUG

#ifdef _EGDEBUG
#define    EgAssert(e)			if(!(e)){ __asm int 3};
#else
#define    EgAssert(e)			((void)0)
#endif

#include "Constants.h"
#define __EG_TCP_DRIVER_IMPORT__
#include <EgTcpDriver\EgTcpDriver.h>

#include "Trace.h"

typedef WINADVAPI BOOL ( WINAPI *LPFN_ChangeServiceConfig2)(
    SC_HANDLE    hService,
    DWORD        dwInfoLevel,
    LPVOID       lpInfo);	

#ifdef UNICODE
#define ChangeServiceConfig2Name "ChangeServiceConfig2W"
#else
#define ChangeServiceConfig2Name "ChangeServiceConfig2A"
#endif// UNICODE

#include "Transport.h"

#endif // !defined(__STD_AFX_H__)
