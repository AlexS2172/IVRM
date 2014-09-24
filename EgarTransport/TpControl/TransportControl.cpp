#include "stdafx.h"
#include "TransportControl.h"
#include <initguid.h>
#include "MainWnd.h"

const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down

_ATL_FUNC_INFO CTpControlApp::m_OnServiceStatus = {CC_STDCALL, VT_EMPTY, 1, {VT_I4} };
_ATL_FUNC_INFO CTpControlApp::m_OnServiceError = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BSTR} };
_ATL_FUNC_INFO CTpControlApp::m_OnLogoned = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CTpControlApp::m_OnLogout = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CTpControlApp::m_OnDisabled = {CC_STDCALL, VT_EMPTY, 0 };

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0)
    {
        bActivity = true;
        SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
    }
    return l;
}

//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
    while (1)
    {
        WaitForSingleObject(hEventShutdown, INFINITE);
        DWORD dwWait=0;
        do
        {
            bActivity = false;
            dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
        } while (dwWait == WAIT_OBJECT_0);
        // timed out
        if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
        {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
            CoSuspendClassObjects();
            if (!bActivity && m_nLockCnt == 0)
#endif
                break;
        }
    }
    CloseHandle(hEventShutdown);
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (h != NULL);
}

CExeModule _Module;

LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

CTpControlApp::CTpControlApp()  : m_TransportStatus(enHSDisabled)
{
}

CTpControlApp theApp;

BEGIN_MESSAGE_MAP(CTpControlApp, CWinApp)
	//{{AFX_MSG_MAP(CTpControlApp)
	ON_COMMAND(ID_TP_START, OnTpStart)
	ON_UPDATE_COMMAND_UI(ID_TP_START, OnUpdateTpStart)
	ON_COMMAND(ID_TP_STOP, OnTpStop)
	ON_UPDATE_COMMAND_UI(ID_TP_STOP, OnUpdateTpStop)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

BOOL CTpControlApp::InitInstance()
{
	LPCTSTR szMutex = _T("EGAR Transport Service Control 8688A43D-5593-48b8-BBC6-235BF42586E1");
	m_hSingleInstanceMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, szMutex);
	if (m_hSingleInstanceMutex)
		return FALSE;

	m_hSingleInstanceMutex = ::CreateMutex(NULL, FALSE, szMutex);

	
	// Initialize COM library
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
    HRESULT hRes = CoInitialize(NULL);	
#endif

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif
	

	for(int i = 0; i < 5; i++)
	{
		hRes = AttachToHost();
		if(SUCCEEDED(hRes))
			break;

		::Sleep(2000L);
	}
	if(FAILED(hRes))
	{
		CString strError;
		strError.Format(_T("Can't access Egar Transport service. (0x%X)"),hRes);
		::AfxMessageBox(strError,MB_OK | MB_ICONSTOP);
		return FALSE;
	}	

	m_hAppIcon[0] = LoadIcon(IDI_SERVICE_OFFLINE);
	m_hAppIcon[1] = LoadIcon(IDI_SERVICE_ONLINE);
	
	WSADATA	wsaData;
	WORD	wVersion = MAKEWORD(2,2);


    int nRet = WSAStartup(wVersion, &wsaData); 
	if (nRet != 0)
		return FALSE;

	if(InitIPAddresses())
		return FALSE;


	CMainWnd* pMainWnd = new CMainWnd;
		
	if (!pMainWnd->LoadFrame(IDR_MAIN_WND))
		return FALSE;
	m_pMainWnd = pMainWnd;

	_Module.StartMonitor();

	return TRUE;
}

int CTpControlApp::ExitInstance()
{
	// MFC's class factories registration is
	// automatically revoked by MFC itself.
	
	int iErr = WSACleanup();
	if (iErr == SOCKET_ERROR)
		return WSAGetLastError();
	
	if (m_pMainWnd)
		m_pMainWnd->DestroyWindow();

	DetachFromHost();

    CoUninitialize();

	if(m_hSingleInstanceMutex)
		::CloseHandle(m_hSingleInstanceMutex);

	return 0;
} 

DWORD CTpControlApp::GetRouterHostName(_bstr_t& bstrHostName)
{
	try
	{
		bstrHostName = _T("");

		CEgRegKey key;
		LONG lResult;

		lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_READ);
			
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.QueryValue (bstrHostName,
				SETTINGS_NETWORK_ROUTER_HOST);
		}
		
		if(lResult != ERROR_SUCCESS)
		{
			bstrHostName = _T("localhost"); // use 'localhost' by default.
		}
		return lResult;
	}
	catch(_com_error er)
	{
		return -1;
	}
}

DWORD CTpControlApp::SetRouterHostName(_bstr_t bstrHostName)
{
	CEgRegKey key;

	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (bstrHostName,
			SETTINGS_NETWORK_ROUTER_HOST);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_NETWORK_REGISTRY_KEY);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (bstrHostName,
				SETTINGS_NETWORK_ROUTER_HOST);
		}
	}
	return lResult;
}

DWORD CTpControlApp::GetSettingsFromReg(LPCTSTR ValueName,DWORD dwDefaultValue,LPCTSTR KeyName)
{
	CEgRegKey key;
	
	DWORD Value;
	LONG lResult;

	lResult = key.Open(HKEY_LOCAL_MACHINE, KeyName, KEY_READ);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.QueryValue (Value, ValueName);
	}

	if (lResult != ERROR_SUCCESS)
	{
		Value = dwDefaultValue;
	}
	return Value;
}

DWORD CTpControlApp::SetSettingsToReg(LPCTSTR ValueName,DWORD dwValue,LPCTSTR KeyName)
{
	CEgRegKey key;
	
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, KeyName, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (dwValue, ValueName);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, KeyName);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (dwValue, ValueName);	
		}
	}

	return lResult;
}

CString CTpControlApp::GetSettingsFromReg(LPCTSTR ValueName,CString strDefaultValue,LPCTSTR KeyName)
{
	CEgRegKey key;
	
	CString Value;
	LONG lResult;

	lResult = key.Open(HKEY_LOCAL_MACHINE, KeyName, KEY_READ);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.QueryValue (Value, ValueName);
	}

	if (lResult != ERROR_SUCCESS)
	{
		Value = strDefaultValue;
	}
	return Value;
}

DWORD CTpControlApp::SetSettingsToReg(LPCTSTR ValueName,CString strValue,LPCTSTR KeyName)
{
	CEgRegKey key;
	
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, KeyName, KEY_WRITE);
	
	if (lResult == ERROR_SUCCESS)
	{
		lResult = key.SetValue (strValue, ValueName);
	}
	else
	{
		lResult = key.Create(HKEY_LOCAL_MACHINE, KeyName);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = key.SetValue (strValue, ValueName);	
		}
	}

	return lResult;
}

STDMETHODIMP CTpControlApp::OnServiceStatus(long dwStatus)
{
	setTpSrvStatus(dwStatus);

	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_TRANSPORT_SERVICE_STATUS);

	return S_OK;
}

STDMETHODIMP CTpControlApp::OnServiceError(long nCode, BSTR bsDescription)
{

	try
	{
		if(m_pMainWnd)
		{
			CString text;
			_bstr_t Descr = bsDescription;
		
			text.Format(_T("Error %d. %s"),nCode, (LPCTSTR)Descr);

			LPARAM lParam =  (LPARAM)(LPCTSTR)text;
			m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_ERROR,0, lParam);		
		}

	}
	catch( const _com_error& e )
	{
		return e.Error();
	}
	return S_OK;
}

STDMETHODIMP CTpControlApp::OnLogoned(BSTR bsRouterName)
{
	setTpStatus(enHSOnline);
	try
	{
		if(m_pMainWnd)
		{
			m_pMainWnd->PostMessage(WM_COMMAND, RT_STATUS_ONLINE, 0);
		
			CString text;
			_bstr_t RouterName = bsRouterName;
		
			CString format;
			format.LoadString(IDS_ONLINE);
			text.Format(format,(LPCTSTR)RouterName);

			m_pMainWnd->SetWindowText(text);
		}

	}
	catch( const _com_error& e )
	{
		return e.Error();
	}
	return S_OK;
}

STDMETHODIMP CTpControlApp::OnLogout(BSTR bsRouterName)
{
	setTpStatus(enHSOffline);
	try
	{
		if(m_pMainWnd)
		{
			m_pMainWnd->PostMessage(WM_COMMAND, RT_STATUS_OFFLINE, 0);
		
			CString text;
			_bstr_t RouterName = bsRouterName;
		
			CString format;

			if(getTpSrvStatus() == SERVICE_RUNNING)
			{
				format.LoadString(IDS_OFFLINE_START);
				text.Format(format,(LPCTSTR)RouterName);
			}
			else
			{
				text.LoadString(IDS_OFFLINE_STOP);
			}

			m_pMainWnd->SetWindowText(text);
		}
	}
	catch( const _com_error& e )
	{
		return e.Error();
	}
	return S_OK;
}
STDMETHODIMP CTpControlApp::OnDisabled()
{
	setTpStatus(enHSDisabled);
	try
	{

		m_ulUnsentMessNum = 0;
		m_ulRecMessNum = 0;
		m_ulSendMessNum = 0;
		m_dtLogonTime = 0;
		m_ulConfMessNum = 0;

		if(m_pMainWnd)
		{
			m_pMainWnd->PostMessage(WM_COMMAND, TRANSPORT_DISABLED, 0);
		
			CString text;
			if(getTpSrvStatus() == SERVICE_RUNNING)
			{
				text.LoadString(IDS_DISABLED_START);
			}
			else
			{
				text.LoadString(IDS_DISABLED_STOP);
			}

			m_pMainWnd->SetWindowText(text);
		}
	}
	catch( const _com_error& e )
	{
		return e.Error();
	}
	return S_OK;
}
void CTpControlApp::GetStatistic()
{
	try
	{	
		if(m_spHost == NULL)
			return;
		if(DoesTransportServiceStarted())
		{
			IHostStatisticDataPtr spData;	

			_com_util::CheckError(m_spHost->get_Statistic(&spData));

			if(spData == NULL)
				return;

			_com_util::CheckError(spData->get_UnsentMessageNum(&m_ulUnsentMessNum));
			_com_util::CheckError(spData->get_ConfirmedMessageNum(&m_ulConfMessNum));
			_com_util::CheckError(spData->get_ReceivedMessageNum(&m_ulRecMessNum));
			_com_util::CheckError(spData->get_SentMessageNum(&m_ulSendMessNum));
			_com_util::CheckError(spData->get_LogonTime(&m_dtLogonTime));

			BSTR bsRouter;
			_com_util::CheckError(spData->get_HostName(&bsRouter));
			m_bsRouterName.Attach(bsRouter);
		}
		
	}
	catch( const _com_error& )
	{
	}
}
HRESULT CTpControlApp::AttachToHost()
{
	if(m_spHost != NULL)
		return S_OK;

	HRESULT hr = m_spHost.CreateInstance(CLSID_HostProxy);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = _IHostProxyEventsImpl::DispEventAdvise(m_spHost);
	if(FAILED(hr))
	{
		m_spHost = NULL;
	}

	return hr;
}
void CTpControlApp::DetachFromHost()
{
	if(m_spHost != NULL)
	{
		_IHostProxyEventsImpl::DispEventUnadvise(m_spHost);
		m_spHost = NULL;
	}

	m_ulConfMessNum = 0;
	m_ulRecMessNum = 0;
	m_ulSendMessNum = 0;
	m_dtLogonTime = 0;
	m_ulUnsentMessNum = 0;
}

bool CTpControlApp::DoesTransportServiceStarted()
{
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, STANDARD_RIGHTS_REQUIRED|SC_MANAGER_CONNECT|SC_MANAGER_ENUMERATE_SERVICE|SC_MANAGER_QUERY_LOCK_STATUS);
	if(scHandle != NULL)
	{
		SC_HANDLE scService = OpenService(scHandle, "Egar Transport", SERVICE_QUERY_STATUS);
		if(scService!=NULL)
		{
			SERVICE_STATUS svcStatus;
			ZeroMemory(&svcStatus,sizeof(svcStatus));
			QueryServiceStatus(scService,&svcStatus);
			if(svcStatus.dwCurrentState!=SERVICE_RUNNING)
				return false;
		}
	}
	return true;
}
HRESULT CTpControlApp::GetHostStatus(CString& strRouterName)
{
	if(m_spHost == NULL)
		return E_FAIL;

	HRESULT hRes = S_OK;
	if(DoesTransportServiceStarted())
	{
		HostStatusEnum Status;
		hRes = m_spHost->get_Status(&Status);
		if(FAILED(hRes))
			return hRes;

		setTpStatus(Status);

		if(Status != enHSDisabled)
		{
			BSTR bsRouterName = NULL;

			hRes = m_spHost->get_RouterName(&bsRouterName);

			if(FAILED(hRes))
				return hRes;

			strRouterName = bsRouterName;

			SysFreeString(bsRouterName);
		}
	}
	return hRes;
}

void CTpControlApp::OnTpStart() 
{
	if(getTpSrvStatus() != SERVICE_STOPPED)
		return;

	setTpSrvStatus(SERVICE_START_PENDING);
	if(m_pMainWnd)
		m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_STATUS);

	if(m_spHost != NULL)
	{
		try
		{
			_com_util::CheckError(m_spHost->Start());
		}
		catch( const _com_error& e)
		{
			if(m_pMainWnd)
			{
				CString text;
				_bstr_t Descr = e.Description();
				
				if(Descr.length())
					text.Format(_T("%s %s"), e.ErrorMessage(), (LPCTSTR)Descr);
				else
					text.Format(_T("%s"), e.ErrorMessage());
			
				LPARAM lParam =  (LPARAM)(LPCTSTR)text;
				m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_ERROR,0, lParam);
			}
		}
	}
}

void CTpControlApp::OnUpdateTpStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(getTpSrvStatus() == SERVICE_STOPPED);
}

void CTpControlApp::OnTpStop() 
{
	if(getTpSrvStatus() != SERVICE_RUNNING)
		return;

	setTpSrvStatus(SERVICE_STOP_PENDING);
	if(m_pMainWnd)
		m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_STATUS);
	if(m_spHost != NULL)
	{
		try
		{
			_com_util::CheckError(m_spHost->Stop());
		}
		catch( const _com_error& e)
		{
			if(m_pMainWnd)
			{
				CString text;
				_bstr_t Descr = e.Description();
			
				if(Descr.length())
					text.Format(_T("%s %s"), e.ErrorMessage(), (LPCTSTR)Descr);
				else
					text.Format(_T("%s"), e.ErrorMessage());
			
				LPARAM lParam =  (LPARAM)(LPCTSTR)text;
				m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_ERROR,0, lParam);
			}
		}
	}
}

void CTpControlApp::OnUpdateTpStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(getTpSrvStatus() == SERVICE_RUNNING);
}

void CTpControlApp::RestartTransportService()
{
	if(getTpSrvStatus() != SERVICE_RUNNING)
		return;

	setTpSrvStatus(SERVICE_STOP_PENDING);

	if(m_pMainWnd)
		m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_STATUS);

	if(m_spHost != NULL)
	{
		try
		{
			_com_util::CheckError(m_spHost->Restart());
		}
		catch( const _com_error& e)
		{
			if(m_pMainWnd)
			{
				CString text;
				_bstr_t Descr = e.Description();
			
				if(Descr.length())
					text.Format(_T("%s %s"), e.ErrorMessage(), (LPCTSTR)Descr);
				else
					text.Format(_T("%s"), e.ErrorMessage());
			
				LPARAM lParam =  (LPARAM)(LPCTSTR)text;
				m_pMainWnd->SendMessage(WM_TRANSPORT_SERVICE_ERROR,0, lParam);
			}
		}
	}
}
 
