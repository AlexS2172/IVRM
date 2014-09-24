// RtControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RtControl.h"

#include "MainFrm.h"
#include "RtControlDoc.h"
#include "RtControlView.h"
#include <initguid.h>
#include "ModulVer.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	APP_KEY		_T("Settings")

_ATL_FUNC_INFO CRtControlApp::m_OnServiceStatus = {CC_STDCALL, VT_EMPTY, 1, {VT_I4} };
_ATL_FUNC_INFO CRtControlApp::m_OnServiceError = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BSTR} };
_ATL_FUNC_INFO CRtControlApp::m_OnClientConnected = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CRtControlApp::m_OnClientDisconnected = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };

/////////////////////////////////////////////////////////////////////////////
// CRtControlApp

BEGIN_MESSAGE_MAP(CRtControlApp, CWinApp)
	//{{AFX_MSG_MAP(CRtControlApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_RT_OPEN, OnRtOpen)
	ON_COMMAND(ID_RT_START, OnRtStart)
	ON_UPDATE_COMMAND_UI(ID_RT_START, OnUpdateRtStart)
	ON_COMMAND(ID_RT_STOP, OnRtStop)
	ON_UPDATE_COMMAND_UI(ID_RT_STOP, OnUpdateRtStop)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRtControlApp construction

CRtControlApp::CRtControlApp() : m_hSCM(0)
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRtControlApp object

CRtControlApp g_App;

/////////////////////////////////////////////////////////////////////////////
// CRtControlApp initialization

BOOL CRtControlApp::InitInstance()
{
	LPCTSTR szMutex = _T("EGAR Router Server Control 66D027D5-7F96-4F72-86E8-3A9CE8746E50");
	m_hSingleInstanceMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, szMutex);
	if (m_hSingleInstanceMutex)
		return FALSE;

	m_hSingleInstanceMutex = ::CreateMutex(NULL, FALSE, szMutex);

#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = ::CoInitialize(NULL);
#endif
	if(FAILED(hRes))
		return FALSE;

	AfxEnableControlContainer();

	if(__argc > 0)
	{
		m_strExeName = __targv[0];
	}
	else
	{
		TCHAR szBuffer[2048] = { 0 };
		if(::GetModuleFileName(m_hInstance, szBuffer, 2047))
		{
			m_strExeName = szBuffer;
		}
	}
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);

	//if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	//{
	//	return TRUE;
	//}


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	SetRegistryKey(_T("EGAR\\ETS"));

	LoadString(m_hInstance, IDS_SERVICENAME, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if(!m_hSCM)
		return FALSE;

	LoadSettings();

	m_bExitNow = FALSE;
	m_nRtStatus = 0;

	m_strStatus[0] = _T("Offline - Router Server Control");
	m_strStatus[1] = _T("Online - Router Server Control");
	m_hSmallIcon[0] = LoadIcon(IDI_OFFLINE_SMALL);
	m_hSmallIcon[1] = LoadIcon(IDI_ONLINE_SMALL);
	m_hAppIcon[0] = LoadIcon(IDR_MAINFRAME2);
	m_hAppIcon[1] = LoadIcon(IDR_MAINFRAME);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CRtControlDoc),
											RUNTIME_CLASS(CMainFrame), RUNTIME_CLASS(CRtControlView));
	AddDocTemplate(pDocTemplate);

	UINT uiFlag = SWP_NOSIZE;
	if(m_ptMainWinPos.x <= 0 && m_ptMainWinPos.y <= 0)
	{
		uiFlag |= SWP_NOMOVE;
	}

	m_nCmdShow = SW_HIDE;
	OnFileNew();
	if(m_nStartMin)
	{
		m_pMainWnd->ModifyStyleEx(0, WS_EX_TOOLWINDOW, SWP_NOZORDER | SWP_NOACTIVATE);
		uiFlag |= SWP_NOREDRAW | SWP_NOZORDER | SWP_NOACTIVATE;
		m_pMainWnd->SetWindowPos(NULL, m_ptMainWinPos.x, m_ptMainWinPos.y, 0, 0, uiFlag);
	}
	else
	{
		uiFlag |= SWP_SHOWWINDOW;
		m_pMainWnd->SetWindowPos(NULL, m_ptMainWinPos.x, m_ptMainWinPos.y, 0, 0, uiFlag);

		//m_pMainWnd->ShowWindow(m_nCmdShow);
		m_pMainWnd->UpdateWindow();
	}

	WSADATA	wsaData;
	WORD	wVersion = MAKEWORD(2,2);

    int nRet = WSAStartup(wVersion, &wsaData);
	if(nRet != 0)
		return FALSE;
	
	m_HostResolveThread.Start();

	for(int i = 0; i < 5; i++)
	{
		hRes = AttachToRouter();
		if(SUCCEEDED(hRes))
			break;

		::Sleep(2000L);
	}
	if(FAILED(hRes))
	{
		CString strError;
		strError.Format(_T("Can't access Egar Router service. (0x%X)"),hRes);
		::AfxMessageBox(strError,MB_OK | MB_ICONSTOP);
		return FALSE;
	}	
	
	return TRUE;
}

void CRtControlApp::LoadSettings()
{
	m_nStartMin = GetProfileInt(APP_KEY, _T("StartMin"), 0) ? 1 : 0;
	m_ptMainWinPos.x = GetProfileInt(APP_KEY, _T("MainWinLeft"), -1);
	m_ptMainWinPos.y = GetProfileInt(APP_KEY, _T("MainWinTop"), -1);
	if(!m_hSCM)
		return;

	SC_HANDLE hService = ::OpenService(m_hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
	
	if(!hService)
	{
		m_nAutoStart = -1;
		return;
	}

	BOOL bRet = FALSE;
	DWORD Size = 1;
	DWORD SizeNeed;
	QUERY_SERVICE_CONFIG* pBuf = NULL;
	pBuf = (QUERY_SERVICE_CONFIG*)LocalAlloc(LPTR,Size);

	bRet = ::QueryServiceConfig(hService,pBuf,Size,&SizeNeed);
	if(!bRet)
	{
		pBuf = (QUERY_SERVICE_CONFIG*)LocalReAlloc(pBuf,SizeNeed,LMEM_MOVEABLE);
		bRet = ::QueryServiceConfig(hService,pBuf,SizeNeed,&SizeNeed);
	}
	
	if(bRet)
	{
		if(pBuf->dwStartType == SERVICE_AUTO_START)
			m_nAutoStart = 1;
		else if(pBuf->dwStartType == SERVICE_DEMAND_START)
			m_nAutoStart = 0;
		else
			m_nAutoStart = -1;
	}

	if(pBuf)
		LocalFree(pBuf);

	if(hService)
		::CloseServiceHandle(hService);
}

void CRtControlApp::SaveSettings()
{
	WriteProfileInt(APP_KEY, _T("StartMin"), m_nStartMin);

	WriteProfileInt(APP_KEY, _T("MainWinLeft"), m_ptMainWinPos.x);
	WriteProfileInt(APP_KEY, _T("MainWinTop"), m_ptMainWinPos.y);
	SaveStartType();
}

void CRtControlApp::SaveStartType()
{
	if(m_nAutoStart == -1)
		return;

	SC_HANDLE hService = ::OpenService(m_hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
	
	if(!hService)
	{
		return;
	}
	
	::ChangeServiceConfig (hService, SERVICE_NO_CHANGE ,
		m_nAutoStart == 1 ? SERVICE_AUTO_START : SERVICE_DEMAND_START,
		SERVICE_NO_CHANGE,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

	if(hService)
		::CloseServiceHandle(hService);
}

int CRtControlApp::ExitInstance()
{
	SaveSettings();

	m_HostResolveThread.Stop();

	int iErr = WSACleanup();
	
	if(m_hSCM)
	{
		::CloseServiceHandle(m_hSCM);
		m_hSCM = NULL;
	}

	DetachFromRouter();

	::CoUninitialize();

	if(m_hSingleInstanceMutex)
		::CloseHandle(m_hSingleInstanceMutex);

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CRtControlApp message handlers

void CRtControlApp::OnRtOpen() 
{
	m_pMainWnd->ModifyStyleEx(WS_EX_TOOLWINDOW, 0);
	m_pMainWnd->ShowWindow(SW_SHOW);
}

void CRtControlApp::OnRtStart() 
{
	if(m_nRtStatus != SERVICE_STOPPED)
		return;

	if(m_spRT != NULL)
		m_spRT->Start();

	m_nRtStatus = SERVICE_START_PENDING;

	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_ROUTER_UPDATE_STATUS);
}

void CRtControlApp::OnUpdateRtStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nRtStatus == SERVICE_STOPPED);
}

void CRtControlApp::OnRtStop() 
{
	if(m_nRtStatus != SERVICE_RUNNING)
		return;

	if(m_spRT != NULL)
		m_spRT->Stop();

	m_nRtStatus = SERVICE_STOP_PENDING;
	
	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_ROUTER_UPDATE_STATUS);
}

void CRtControlApp::OnUpdateRtStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nRtStatus == SERVICE_RUNNING);
}

void CRtControlApp::OnAppExit() 
{
	if(m_pMainWnd->GetSafeHwnd())
	{
		CRect rc;
		m_pMainWnd->GetWindowRect(rc);
		m_ptMainWinPos = rc.TopLeft();
	}
	m_bExitNow = TRUE;
	CWinApp::OnAppExit();
}

STDMETHODIMP CRtControlApp::OnServiceStatus(long dwStatus)
{
	m_nRtStatus = dwStatus;

	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_ROUTER_UPDATE_STATUS);

	return S_OK;
}

STDMETHODIMP CRtControlApp::OnServiceError(long nCode, BSTR bsDescription)
{

	try
	{
		if(m_pMainWnd)
		{
			CString text;
			_bstr_t Descr = bsDescription;
		
			text.Format(_T("Error %d. %s"),nCode, (LPCTSTR)Descr);

			LPARAM lParam =  (LPARAM)(LPCTSTR)text;
			m_pMainWnd->SendMessage(WM_ROUTER_SERVICE_ERROR,0, lParam);		
		}

	}
	catch( const _com_error& e )
	{
		return e.Error();
	}
	return S_OK;
}

STDMETHODIMP CRtControlApp::OnClientConnected(BSTR bsHostIP)
{
	unsigned long addr;
	try
	{
		addr = inet_addr(_bstr_t(bsHostIP));
	}
	catch(_com_error)
	{		
		addr = INADDR_NONE;
	}

	if(INADDR_NONE != addr)
	{		
		m_HostResolveThread.ResolveHostName(addr,true);
	}
	return S_OK;
}

STDMETHODIMP CRtControlApp::OnClientDisconnected(BSTR bsHostIP)
{
	unsigned long addr;
	try
	{
		addr = inet_addr(_bstr_t(bsHostIP));
	}
	catch(_com_error)
	{		
		addr = INADDR_NONE;
	}

	if(INADDR_NONE != addr)
	{		
		m_HostResolveThread.ResolveHostName(addr,false);
	}
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD)
{
	TCHAR	szFileName[1024];
	CString sFmt, sVer = _T("N/A");

	if (GetModuleFileName(g_App.m_hInstance, szFileName, sizeof(szFileName)))
	{
		CModuleVersion ver;
		ver.GetModuleVersionInfo(szFileName);
	
		ver.GetValue(_T("FileVersion"), sVer);
		
		INT iMajor, iMinor, iBuild, iRevision;
		if (_stscanf_s(sVer, _T("%d,%d,%d,%d"), &iMajor, &iMinor, &iBuild, &iRevision) == 4)
		{
			sFmt.Format(_T("%d, %d, %d, %d"),
						iMajor, iMinor, iBuild, iRevision);
		}
	}
	
	CString strFile,strServiceFile,strVer,strProxyFile,strVer2;

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile (szFileName, &data);
	if( hFind != INVALID_HANDLE_VALUE )
	{
		FindClose(hFind);

		strFile = data.cFileName;					
	}					
	
	FillObjectVersion (__uuidof (Router),strServiceFile,strVer);

	FillObjectVersion (__uuidof (RouterProxy),strProxyFile,strVer2);
	
	//{{AFX_DATA_INIT(CAboutBox)
	m_sVersion = sFmt;
	m_sVersion2 = strVer;
	m_sVersion3 = strVer2;
	m_sServiceName = strServiceFile;
	m_sProxyName = strProxyFile;
	m_sName = strFile;
	//}}AFX_DATA_INIT
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	DDX_Text(pDX, IDC_VERSION2, m_sVersion2);
	DDX_Text(pDX, IDC_VERSION3, m_sVersion3);
	DDX_Text(pDX, IDC_PROXY_NAME, m_sProxyName);
	DDX_Text(pDX, IDC_SERVICE_NAME, m_sServiceName);
	DDX_Text(pDX, IDC_NAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers


void CAboutBox::FillObjectVersion(LPTSTR szProgID,CString& strFile, CString& strVer)
{
	CLSID cls;
	USES_CONVERSION;
	CLSIDFromProgID(T2W(szProgID), &cls);
	FillObjectVersion(cls,strFile,strVer);
}

void CAboutBox::FillObjectVersion(CLSID clsid,CString& strFile, CString& strVer)
{
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (clsid, &szCLSID);
	
	CString strKey = _T("CLSID\\");
	strKey += CString  (szCLSID);
	CoTaskMemFree (szCLSID);

	CRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			CString strVal;
			LPTSTR szVal = strVal.GetBuffer(MAX_PATH);
			DWORD dwSize = MAX_PATH;
			keyModule.QueryStringValue(_T(""), szVal, &dwSize);
			strVal.ReleaseBuffer();

			if (!strVal.IsEmpty() )
			{
				CModuleVersion ver;
				strVer = _T("N/A");

				if (ver.GetFileVersionInfo (strVal))
					ver.GetValue(_T("FileVersion"), strVer);
				
				WIN32_FIND_DATA data;
				HANDLE hFind = FindFirstFile (strVal, &data);
				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);

					strFile = data.cFileName;					
				}					
			}
		}

		key.Close();
	}
}


// App command to run the dialog
void CRtControlApp::OnAppAbout()
{
	CAboutBox aboutDlg;
	aboutDlg.DoModal();
}
	
CRtControlModule _Module;

//BEGIN_OBJECT_MAP(ObjectMap)
//END_OBJECT_MAP()

LONG CRtControlModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CRtControlModule::Lock()
{
	AfxOleLockApp();
	return 1;
}

LPCTSTR CRtControlModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

HRESULT CRtControlApp::AttachToRouter()
{

	if(m_spTP != NULL)
		return S_OK;

	HRESULT hr = m_spTP.CreateInstance(CLSID_HostProxy);
	if(FAILED(hr))
	{
		return hr;
	}

	if(m_spRT != NULL)
		return S_OK;

	hr = m_spRT.CreateInstance(CLSID_RouterProxy);
	if(FAILED(hr))
	{
		return hr;
	}


	hr = _IRouterEventsImpl::DispEventAdvise(m_spRT);
	if(FAILED(hr))
	{
		m_spRT = NULL;
	}
	
	GetActiveHosts();

	return hr;
}
void CRtControlApp::DetachFromRouter()
{
	if(m_spRT != NULL)
	{
		_IRouterEventsImpl::DispEventUnadvise(m_spRT);
		m_spRT = NULL;
	}
	if(m_spTP != NULL)
		m_spTP =NULL;
}

bool CRtControlApp::GetHostStatistic(BSTR bsHostIP)
{
	try
	{	
		if(m_spRT == NULL)
			return false;

		IHostStatisticDataPtr spData;	

		_com_util::CheckError(m_spRT->get_HostStatistic(bsHostIP,&spData));
		
		if(spData == NULL)
			return false;

		_com_util::CheckError(spData->get_UnsentMessageNum(&m_lUnsentMessNum));
		_com_util::CheckError(spData->get_ConfirmedMessageNum(&m_lConfMessNum));
		_com_util::CheckError(spData->get_ReceivedMessageNum(&m_lRecMessNum));
		_com_util::CheckError(spData->get_SentMessageNum(&m_lSendMessNum));
		_com_util::CheckError(spData->get_LogonTime(&m_dtLogonTime));

	}
	catch( const _com_error& )
	{
		return false;
	}
	return true;
}

void CRtControlApp::GetActiveHosts()
{
	LPSAFEARRAY psa = NULL;
	try
	{
		_com_util::CheckError(m_spRT->get_ActiveHosts(&psa));

		BSTR* pData;

		UINT uiDim = SafeArrayGetDim(psa);
		if(uiDim == 1)
		{
			_com_util::CheckError(SafeArrayAccessData(psa, (PVOID*)&pData));

			for (ULONG iIndex = 0; iIndex < psa->rgsabound[0].cElements; iIndex++)
			{
				unsigned long addr;
				addr = inet_addr(_bstr_t(pData[iIndex]));

				if(INADDR_NONE != addr)
				{		
					m_HostResolveThread.ResolveHostName(addr,true);
				}
			}
		}
		SafeArrayUnaccessData(psa);		
		SafeArrayDestroy(psa);
		
	}
	catch(_com_error)
	{		
		SafeArrayUnaccessData(psa);
		SafeArrayDestroy(psa);
	}
}
