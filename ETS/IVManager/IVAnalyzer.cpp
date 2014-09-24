// IVAnalyzer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include <initguid.h>
#include "IVAnalyzer_i.c"
#include "DataProcessor.h"
#include "OptionsDlg.h"

#include "IVAnalyzerBar.h"
#include "LicenseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerApp

#ifdef _VOLA_MANAGER

LPCTSTR CIVAnalyzerApp::m_szRegKey  = _T("Software\\EGAR\\ETS\\VolaManager\\Settings");
LPCTSTR CIVAnalyzerApp::m_szRegKeyRoot  = _T("Software\\EGAR\\ETS\\VolaManager");

LPCTSTR CIVAnalyzerApp::m_sbsXMLKey = _T("ETS\\VolaManager\\Settings");
LPCTSTR CIVAnalyzerApp::m_sbsXMLKeyRoot = _T("ETS\\VolaManager");
LPCSTR  CIVAnalyzerApp::m_sbsXMLGroupsKeyRoot = _T("ETS\\Asp\\Groups\\");


LPCTSTR CIVAnalyzerApp::m_szRegKeyRootEgar  = _T("Software\\EGAR\\");
LPCTSTR CIVAnalyzerApp::m_szRegKeyRootEgarETS  = _T("ETS");
LPCTSTR CIVAnalyzerApp::m_szRegKeyRootEgarETSVM  = _T("VolaManager");
LPCTSTR CIVAnalyzerApp::m_szRegKeyRootEgarETSVMS  = _T("Settings");

#else

LPCTSTR CIVAnalyzerApp::m_szRegKey  = _T("Software\\EGAR\\Volatility Analyzer\\Settings");
LPCTSTR CIVAnalyzerApp::m_szRegKeyRoot  = _T("Software\\EGAR\\Volatility Analyzer");

#endif //_VOLA_MANAGER

LPCTSTR CIVAnalyzerApp::m_szMyTrackKey = _T("Software\\EGAR\\{FD68F05A-247C-4D19-B12D-852691429151}");

BEGIN_MESSAGE_MAP(CIVAnalyzerApp, CWinApp)
	//{{AFX_MSG_MAP(CIVAnalyzerApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerApp construction

CIVAnalyzerApp::CIVAnalyzerApp():
#ifdef _VOLA_MANAGER
	CWinApp( _T("Volatility Manager") ),
#else
	CWinApp( _T("Volatility Analyzer") ),
#endif //_VOLA_MANAGER
	m_hAccel (NULL), m_hMutex (NULL), m_hCBTHook (NULL),
	m_bExpired (false), m_dThreshold (0.00)
	,m_hAppearanceModule(NULL)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIVAnalyzerApp object

CIVAnalyzerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerApp initialization

BOOL CIVAnalyzerApp::InitInstance()
{
	
	AfxOleInit();
	if (!InitATL())
		return FALSE;

	DWORD dwID = ::GetCurrentThreadId ();
	ATLTRACE ( _T("Main thread: %x\n"), dwID);
	
	setlocale (LC_ALL, "");

	AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE); 
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE); 

	AfxEnableControlContainer();

	m_hAccel = LoadAccelerators (AfxGetResourceHandle(), MAKEINTRESOURCE (IDR_MAIN_ACC));
	ASSERT (m_hAccel);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{
		m_strSymbol = cmdInfo.m_strFileName;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &m_osvi );

	LPCTSTR szMutex = _T("EGAR IVAnalyzer Instance");
	m_hMutex = OpenMutex (MUTEX_ALL_ACCESS, FALSE, szMutex);
	if (m_hMutex)
	{
		if(m_strSymbol.GetLength())
		{
			IVM::IIVControlPtr spCommand;
			if(SUCCEEDED(spCommand.CreateInstance(__uuidof(IVM::IVControl))))
			{
				USES_CONVERSION;
				CComBSTR bs = T2W((LPCTSTR)m_strSymbol);
				HRESULT hr = spCommand->raw_SetActiveSymbol((BSTR)bs);
			}
		}
		return FALSE;
	}

	m_hMutex = CreateMutex (NULL, FALSE, szMutex);

	HRESULT _hr = S_OK;
	if(m_spProvider==NULL)
	{
		_hr = m_spProvider.CreateInstance(__uuidof(PP::Providers));
		if(SUCCEEDED(_hr))
			m_spProvider->Initialize();
	}

#ifdef _VOLA_MANAGER
	HRESULT hr = m_spDBSettings.CreateInstance(__uuidof(Stg::Settings));
	hr = m_spIVSettings.CreateInstance(__uuidof(Stg::IVSettings));
#endif
		
	if (!ReadOptions(FALSE) )
	{
#ifdef _VOLA_MANAGER
		m_spDBSettings=NULL;
		m_spIVSettings=NULL;
#endif
		m_spProvider = NULL;
		return FALSE; 
	}
	
	CIVAnalyzerBar * pBar = new CIVAnalyzerBar;
	m_pMainWnd = pBar;

	if (! pBar->Create() )
	{
		delete pBar;
		m_pMainWnd = NULL;
		return FALSE;
	}

    m_hCBTHook = SetWindowsHookEx (WH_CBT, IVCbtFilterHook, m_hInstance, m_nThreadID);
	
	return TRUE;
}

LRESULT CALLBACK CIVAnalyzerApp::IVCbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code !=  HCBT_ACTIVATE)
		return CallNextHookEx(theApp.m_hCBTHook, code,	wParam, lParam);

	CWnd *pWnd = CWnd::FromHandle ( (HWND)wParam );
	LPCBTACTIVATESTRUCT pStruct = (LPCBTACTIVATESTRUCT)lParam; 
	
	if( (pWnd->GetStyle() & WS_POPUP ) && !pWnd->GetParent() )
	{
		CIVAnalyzerBar *pBar = static_cast<CIVAnalyzerBar*>( theApp.m_pMainWnd );
		if( pBar && pBar->IsBarAlwaysOnTop() )
			pWnd->SetWindowPos (&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}	

	return CallNextHookEx(theApp.m_hCBTHook, code,	wParam, lParam);
}
	
CIVAnalyzerModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CIVAnalyzerModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CIVAnalyzerModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CIVAnalyzerModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
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


int CIVAnalyzerApp::ExitInstance()
{
#ifdef _VOLA_MANAGER
	m_spIVSettings=NULL;
	m_spDBSettings=NULL;
#endif
	m_spProvider = NULL;

	if (m_hCBTHook)
	{
		UnhookWindowsHookEx	(m_hCBTHook);
		m_hCBTHook = NULL;
	}

	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
	}

	if (m_hMutex)
		CloseHandle (m_hMutex);

	if(m_hAppearanceModule)
		FreeLibrary(m_hAppearanceModule);

	return CWinApp::ExitInstance();

}

BOOL CIVAnalyzerApp::InitATL()
{
	m_bATLInited = TRUE;

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	BOOL bRun = TRUE;

#ifdef _VOLA_MANAGER

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

	TCHAR szTokens[] = _T("-/");
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);

	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_IVMANAGER, FALSE);
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_IVMANAGER, TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}
#endif


	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		return FALSE;
	}

	return TRUE;
}

BOOL CIVAnalyzerApp::ProcessMessageFilter(int code, LPMSG lpMsg) 
{
	if (code ==  MSGF_MENU && lpMsg->message == WM_LBUTTONDBLCLK)
	{
		CIVAnalyzerBar * pBar = static_cast <CIVAnalyzerBar*> (m_pMainWnd);
		
		POINTS pts = MAKEPOINTS (lpMsg->lParam);
		CPoint pt (pts.x, pts.y);

		if ( pBar->PtInIcon (pt) )
		{
			pBar->PostMessage (WM_COMMAND, ID_FILE_CLOSE);
		}
	}

	
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

BOOL CIVAnalyzerApp::PreTranslateMessage(MSG* pMsg) 
{
	if ( m_pMainWnd && TranslateAccelerator(*m_pMainWnd, m_hAccel, pMsg) )	
		return FALSE;
	else
		return CWinApp::PreTranslateMessage(pMsg);
}

void CIVAnalyzerApp::GetOptions (CIVAnalyzerOptions & opts)
{
	m_csOptions.Lock();
	
	opts = m_opts;
	
	m_csOptions.Unlock();
}

CString CIVAnalyzerApp::GetConnection()
{
	CString m_strConn;
	
	m_csOptions.Lock();
	m_strConn = m_opts.m_strConnection;	
	m_csOptions.Unlock();

	return m_strConn;
}


bool CIVAnalyzerApp::ReadOptions( bool Flag)
{
	
	CXMLParamsHelper XMLParams;
	if(FAILED(XMLParams.LoadXMLParams()))
	{
		return false;
	}

	TCHAR szUserName[UNLEN + 1];
	DWORD nSize = UNLEN + 1;

	::GetUserName((LPTSTR)&szUserName, &nSize);
	m_strUser = szUserName;

	_bstr_t sbsCompany;
	
	_bstr_t bsKeyGroups(m_szRegKeyRootEgarETS);
	bsKeyGroups += L"\\Asp\\Users\\";
	bsKeyGroups += m_strUser.GetString();

	XMLParams.GetMainXMLString(bsKeyGroups, L"UserGroup", &sbsCompany);


	m_strCompany = sbsCompany.GetBSTR();
	
	m_license.InitUID();

	_bstr_t bsGroupKey (m_sbsXMLGroupsKeyRoot);
	bsGroupKey+=sbsCompany;

	_bstr_t sbsAppearanceLibrary;
	XMLParams.GetMainXMLString(bsGroupKey, L"Appearance", &sbsAppearanceLibrary);
	if(!sbsAppearanceLibrary.length())
		sbsAppearanceLibrary = L"IVRmRes.dll";

	m_hAppearanceModule = LoadLibraryW(sbsAppearanceLibrary);

	m_opts.m_enCurveMode = enOTM;
	m_opts.m_enCalculationMode = enBinominal;
	m_opts.m_enChartMode = enChart_5Penny_Range;
	m_opts.m_fChartRangeFrom = 50.;
	m_opts.m_fChartRangeTo = 150.;
	
#ifndef _VOLA_MANAGER
	lResult = keyLicense.Open (HKEY_CURRENT_USER, CIVAnalyzerApp::m_szRegKeyRoot, KEY_READ);
	if (lResult ==  ERROR_SUCCESS)
	{
		keyLicense.QueryValue (strLicenseKey, _T("LicenseKey"));
		keyLicense.Close();
	}

	while  ( !CheckLicense (strLicenseKey) )
	{
		CString strMessage;
		if (m_bExpired)   
			strMessage.LoadString (IDS_EXPIRED);
		else
			strMessage.LoadString (IDS_NOT_LICENSED);

		AfxMessageBox (strMessage, MB_OK | MB_ICONEXCLAMATION, 0);

		CLicenseDlg dlg;
		dlg.m_strProductID = m_license.GetProductID();
		if (dlg.DoModal() != IDOK)
			return FALSE;
		else
			strLicenseKey = dlg.m_strLicenseKey;
	}
	
	lResult = keyLicense.Create (HKEY_CURRENT_USER, CIVAnalyzerApp::m_szRegKeyRoot);
	if (lResult ==  ERROR_SUCCESS)
	{
		keyLicense.SetValue (strLicenseKey, _T("LicenseKey"));
		keyLicense.Close();
	}
	else
	{
		AfxMessageBox (_T("Unable to write into registry"), MB_OK | MB_ICONEXCLAMATION, 0);
	}
#endif _VOLA_MANAGER

#ifdef _IVAN_DB_ACCESS
	{
		TCHAR szName [MAX_PATH] = _T("");
		GetModuleFileName (AfxGetInstanceHandle(), szName, MAX_PATH);
		
		TCHAR szDrive [MAX_PATH] = _T("");
		TCHAR szDir [MAX_PATH] = _T("");

		_tsplitpath (szName, szDrive, szDir, NULL, NULL);

		m_opts.m_strConnection.Format 
			(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s%sData\\VolaAnalyzer.mdb"), szDrive, szDir);
	}
#endif
	
	bool bShowOptionsDlg = false;

#ifdef _VOLA_MANAGER
	if(m_spDBSettings!=NULL)
	{
#ifdef _MULTIPROVIDERS
		m_opts.m_enProviderType = m_spDBSettings->PriceProvider;
#else
		m_opts.m_enProviderType = 6L;
#endif
#ifndef _IVAN_DB_ACCESS
		m_opts.m_strConnection =(LPCTSTR) m_spDBSettings->DatabaseConnection;
#endif
	}
#endif

#ifdef _VOLA_MANAGER
	if(m_spIVSettings!=NULL)
	{
		CIVOptions & opts = m_opts.m_IVOpts;
		opts.m_strURL    =(LPCTSTR) m_spIVSettings->VolServerName;
		opts.m_strURL    +=_T("/");
		opts.m_strURL    +=(LPCTSTR) m_spIVSettings->VolServerAppPath;

		opts.m_strAccount = (LPCTSTR) m_spIVSettings->VolLogin;
		opts.m_strPassword=(LPCTSTR) m_spIVSettings->VolPassword;
		opts.m_nUseProxy= (DWORD) m_spIVSettings->UseProxy;
		opts.m_strProxy=(LPCTSTR) m_spIVSettings->ProxyName;
		opts.m_nPort=(DWORD) m_spIVSettings->ProxyPort;

		opts.m_strProxyLogin=(LPCTSTR) m_spIVSettings->ProxyLogin;
		opts.m_strProxyPassword=(LPCTSTR) m_spIVSettings->ProxyPassword;
		opts.m_iProxyAuth=(DWORD) m_spIVSettings->ProxyAuthType;
	}
#endif

	_bstr_t sbsThreshold;
	XMLParams.GetXMLString(m_sbsXMLKey, L"Threshold", &sbsThreshold);
	CString strThreshold = sbsThreshold.GetBSTR();

	{	
		if (!strThreshold.IsEmpty())
		{
			float fThreshold = 0;
			setlocale( LC_NUMERIC, "English" );
			_stscanf_s (strThreshold, _T("%f"), &fThreshold);
			setlocale( LC_ALL, "" );

			m_dThreshold = fabs(fThreshold);
		}

		XMLParams.GetXMLLong(m_sbsXMLKey, L"CalcMode", (long*)&m_opts.m_enCalculationMode, 0);
		XMLParams.GetXMLLong(m_sbsXMLKey, L"CurveMode", (long*)&m_opts.m_enCurveMode, 2);
		XMLParams.GetXMLLong(m_sbsXMLKey, L"ChartMode", (long*)&m_opts.m_enChartMode, 3);

		long nValue = 0;
		XMLParams.GetXMLLong(m_sbsXMLKey, L"FlatOutRange", &nValue, 1);
		m_opts.m_bFlatOutRange = (nValue != 0);
		XMLParams.GetXMLLong(m_sbsXMLKey, L"FlatNoBid", &nValue, 1);
		m_opts.m_bFlatNoBid = (nValue != 0);
		
		m_opts.m_enProviderType = 6;

#ifdef _MULTIPROVIDERS
#ifdef _VOLA_MANAGER
		if(m_spDBSettings==NULL)
#endif
		{
			long nType = -1;
			XMLParams.GetXMLLong(m_sbsXMLKey, L"PriceProvider", &nType, -1);
			if(nType != -1)
				m_opts.m_enProviderType = nType;
		}

#else
		m_opts.m_enProviderType = 6;
#endif
		if(m_opts.m_enProviderType != /*enMyTrackPriceProvider*/0)
			bShowOptionsDlg = false;

		long nFrom = 0;
		long nTo = 0;
		XMLParams.GetXMLLong(m_sbsXMLKey, L"ChartRangeFrom", &nFrom, 0);
		XMLParams.GetXMLLong(m_sbsXMLKey, L"ChartRangeTo", &nTo, 0);

		m_opts.m_fChartRangeFrom = double(nFrom)/100.;
		m_opts.m_fChartRangeTo = double(nTo)/100.;

		long nVMAlwaysOnTop = 1;
		XMLParams.GetXMLLong(m_sbsXMLKey, L"VMAlwaysOnTop", &nVMAlwaysOnTop, 1);
		m_opts.m_bVMAlwaysOnTop = (nVMAlwaysOnTop != 0);

		long nVMUseExtrapolation = 1;
		XMLParams.GetXMLLong(m_sbsXMLKey, L"VMUseExtrapolation", &nVMUseExtrapolation, 1);
		m_opts.m_bVMUseExtrapolation = (nVMUseExtrapolation != 0);

		long nSleepDuration  = 0;
		long nSleepFrequency = 1;
		XMLParams.GetXMLLong(m_sbsXMLKey, L"SleepDuration", &nSleepDuration, 0);
		XMLParams.GetXMLLong(m_sbsXMLKey, L"SleepFrequency", &nSleepFrequency, 1);
		m_opts.m_nSleepDuration  = nSleepDuration;
		m_opts.m_nSleepFrequency = nSleepFrequency;

#ifndef _IVAN_DB_ACCESS
//#ifdef _VOLA_MANAGER						//
//		if(m_spDBSettings==NULL)			// TODO: Uncomment when ETS Manager ASP done
//#endif									//
		{
			///////////////////
			// Get DB for user

			CComBSTR sbsDbName;
			if(SUCCEEDED(XMLParams.GetDbConnect(&sbsDbName)))
				m_opts.m_strConnection = sbsDbName;

			// Get DB for user
			///////////////////
		}		
#endif
	}	
	if (m_opts.m_strConnection.IsEmpty())
	{
		::MessageBox(NULL, "Couldn't get DB Settings", "Error", 0);
		bShowOptionsDlg = true;
	}
	if (!Flag)
	{
		CDBLayout db;

		_bstr_t bsUserName;
		DWORD nSize = UNLEN + 1;
		TCHAR szUserName[UNLEN + 1];
		::GetUserName((LPTSTR)&szUserName, &nSize);
		bsUserName = szUserName;

		_bstr_t bsUserGroup;
		_bstr_t sbsUserGroup;
		_bstr_t sbsXMLKey = DEF_XML_KEY;
		sbsXMLKey += L"\\";
		sbsXMLKey += bsUserName;
		XMLParams.GetMainXMLString(sbsXMLKey, L"UserGroup", &sbsUserGroup);
		bsUserGroup = sbsUserGroup.Detach();

		LPSTR  bsServerName =  new char [50];
		DWORD  ServerNameSize = UNLEN + 1;;
		GetComputerName(bsServerName,&ServerNameSize);
		db.Connect();
		db.UserActionControl (COleDateTime::GetCurrentTime(),4,bsUserName,bsServerName,bsUserGroup);
		db.Disconnect(); 
	}

	CIVOptions & opts = m_opts.m_IVOpts;

#ifdef _VOLA_MANAGER
	if(m_spIVSettings==NULL)
#endif
	{
		_bstr_t sbsXMLKey = m_sbsXMLKey;
		sbsXMLKey += L"\\IVFeed";

		_bstr_t sbsValue;
		XMLParams.GetXMLString(sbsXMLKey, L"URL", &sbsValue);
		opts.m_strURL = sbsValue.GetBSTR();

		XMLParams.GetXMLString(sbsXMLKey, L"Account", &sbsValue);
		opts.m_strAccount = sbsValue.GetBSTR();

		XMLParams.GetXMLString(sbsXMLKey, L"Password", &sbsValue);
		opts.m_strPassword = sbsValue.GetBSTR();

		XMLParams.GetXMLLong(sbsXMLKey, L"UseProxy", (long*)&opts.m_nUseProxy);

		XMLParams.GetXMLString(sbsXMLKey, L"Proxy", &sbsValue);
		opts.m_strProxy = sbsValue.GetBSTR();

		XMLParams.GetXMLLong(sbsXMLKey, L"Port", (long*)&opts.m_nPort);

		XMLParams.GetXMLString(sbsXMLKey, L"ProxyLogin", &sbsValue);
		opts.m_strProxyLogin = sbsValue.GetBSTR();

		XMLParams.GetXMLString(sbsXMLKey, L"ProxyPassword", &sbsValue);
		opts.m_strProxyPassword = sbsValue.GetBSTR();

		XMLParams.GetXMLLong(sbsXMLKey, L"ProxyAuthorization", (long*)&opts.m_iProxyAuth);
	}

	if ( opts.IsEmpty ()  )
	{
		bShowOptionsDlg = true;
		::MessageBox(NULL, "Couldn't get IV Settings", "Error", 0);
	}


	VARIANT_BOOL bInit = VARIANT_FALSE;
	{
		PP::IProviderDataPtr spProviderData;
		if(SUCCEEDED(m_spProvider->raw_GetProvider(m_opts.m_enProviderType, &spProviderData)))
		{
			VARIANT_BOOL bInit = VARIANT_TRUE;
			PP::ProviderUseProxyType  enType;
			_bstr_t bsValue;
			spProviderData->get_NeedLogin(&bInit);
			if(bInit!=VARIANT_FALSE)
			{
				spProviderData->get_UseProxy(&enType);
				m_opts.m_PriceProvOpts.m_iUseProxy = (int)enType;

				bsValue = spProviderData->UserAccount;
				m_opts.m_PriceProvOpts.m_strAccount = (LPCTSTR)bsValue;

				bsValue = spProviderData->UserPassword;
				m_opts.m_PriceProvOpts.m_strPassword = (LPCTSTR)bsValue;

				bsValue = spProviderData->ProxyName;
				m_opts.m_PriceProvOpts.m_strProxy = (LPCTSTR)bsValue;

				m_opts.m_PriceProvOpts.m_iProxyPort = spProviderData->ProxyPort;
			}
			else
			{
				m_opts.m_PriceProvOpts.m_iUseProxy = 0;
			}
		}
	}

	if (bInit && m_opts.m_PriceProvOpts.IsEmpty () )
	{
		::MessageBox(NULL, "Couldn't get PriceProvider Settings", "Error", 0);
		bShowOptionsDlg = true;
	}
		
	
	if (bShowOptionsDlg)
	{
		// No option dialog at ASP
		::MessageBox(NULL, "Volatility Manager Initialization Failed", "Error", 0);	
		return false;
	}

	return true;
}

bool CIVAnalyzerApp::SaveOptions (CIVAnalyzerOptions & opts)
{
	CXMLParamsHelper XMLParams;

	bool bSavePriceAcnt = (opts.m_PriceProvOpts != m_opts.m_PriceProvOpts);
	bool bResult = true;

	if(FAILED(XMLParams.LoadXMLParams()))
		return false;
	CDBLayout db;

	_bstr_t bsUserName;
	DWORD nSize = UNLEN + 1;
	TCHAR szUserName[UNLEN + 1];
	::GetUserName((LPTSTR)&szUserName, &nSize);
	bsUserName = szUserName;

	_bstr_t bsUserGroup;
	_bstr_t sbsUserGroup;
	_bstr_t sbsXMLKey = DEF_XML_KEY;
	sbsXMLKey += L"\\";
	sbsXMLKey += bsUserName;
	XMLParams.GetMainXMLString(sbsXMLKey, L"UserGroup", &sbsUserGroup);
	bsUserGroup = sbsUserGroup.Detach();

	LPSTR  bsServerName =  new char [50];
	DWORD  ServerNameSize = UNLEN + 1;;
	GetComputerName(bsServerName,&ServerNameSize);
	db.Connect();
	db.UserActionControl (COleDateTime::GetCurrentTime(),5,bsUserName,bsServerName,bsUserGroup);
	db.Disconnect();
	
	XMLParams.SetXMLLong(m_sbsXMLKey, L"CurveMode",	opts.m_enCurveMode);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"CalcMode", opts.m_enCalculationMode);

	XMLParams.SetXMLLong(m_sbsXMLKey, L"ChartMode", opts.m_enChartMode);

	XMLParams.SetXMLLong(m_sbsXMLKey, L"ChartRangeFrom", (long)opts.m_fChartRangeFrom*100);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"ChartRangeTo", (long)opts.m_fChartRangeTo*100);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"VMAlwaysOnTop", opts.m_bVMAlwaysOnTop);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"VMUseExtrapolation", opts.m_bVMUseExtrapolation);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"SleepDuration", opts.m_nSleepDuration);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"SleepFrequency", opts.m_nSleepFrequency);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"FlatOutRange", opts.m_bFlatOutRange);
	XMLParams.SetXMLLong(m_sbsXMLKey, L"FlatNoBid", opts.m_bFlatNoBid);

#ifdef _MULTIPROVIDERS
#ifdef _VOLA_MANAGER
	if(m_spDBSettings!=NULL)
		m_spDBSettings->PriceProvider =(DWORD )(opts.m_enProviderType);
	else
#endif
		XMLParams.SetXMLLong(m_sbsXMLKey, L"PriceProvider", opts.m_enProviderType);

#else
	XMLParams.SetXMLLong(m_sbsXMLKey, L"PriceProvider", 0);
#endif

#ifndef _IVAN_DB_ACCESS
	if(m_spDBSettings!=NULL)
		m_spDBSettings->DatabaseConnection = (LPCTSTR)opts.m_strConnection;
	else 
#ifdef _VOLA_MANAGER
#endif
	{
		//CEgRegKey keyDB;
		//keyDB.Create (key, _T("Database"));
		//keyDB.SetValue ( opts.m_strConnection, NULL);
		//keyDB.Close(); 
	}

#endif
	
#ifdef _VOLA_MANAGER
	if(m_spIVSettings!=NULL)
	{
		CIVOptions & optsIV = opts.m_IVOpts;
		m_spIVSettings->VolLogin = (LPCTSTR)optsIV.m_strAccount;
		m_spIVSettings->VolPassword =(LPCTSTR)optsIV.m_strPassword;
		m_spIVSettings->UseProxy =(Stg::UseProxyType)optsIV.m_nUseProxy;
		m_spIVSettings->ProxyName = (LPCTSTR)optsIV.m_strProxy;
		m_spIVSettings->ProxyPort = (LONG)optsIV.m_nPort;

		m_spIVSettings->ProxyLogin = (LPCTSTR)optsIV.m_strProxyLogin;
		m_spIVSettings->ProxyPassword = (LPCTSTR)optsIV.m_strProxyPassword;
		m_spIVSettings->ProxyAuthType = (Stg::ProxyAuthentificationType)optsIV.m_iProxyAuth;
	}
	else
#else
	{
		_bstr_t sbsXMLKeyIVFeed = m_sbsXMLKey;
		sbsXMLKeyIVFeed += L"\\IVFeed";
		
		CIVOptions & optsIV = opts.m_IVOpts;

		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"URL", optsIV.m_strURL);
		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"Account", optsIV.m_strAccount);
		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"Password", optsIV.m_strPassword);
		
		XMLParams.SetXMLLong(sbsXMLKeyIVFeed, L"UseProxy", optsIV.m_nUseProxy);

		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"Proxy", optsIV.m_strProxy);
		XMLParams.SetXMLLong(sbsXMLKeyIVFeed, L"Port", optsIV.m_nPort);

		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"ProxyLogin", optsIV.m_strProxyLogin);
		XMLParams.SetXMLString(sbsXMLKeyIVFeed, L"ProxyPassword", optsIV.m_strProxyPassword);
		XMLParams.SetXMLLong(sbsXMLKeyIVFeed, L"ProxyAuthorization", optsIV.m_iProxyAuth);
	}
#endif

	if (bSavePriceAcnt)	
	{
		PP::IProviderDataPtr spProviderData;
		if(SUCCEEDED(m_spProvider->raw_GetProvider(m_opts.m_enProviderType, &spProviderData)))
		{
//			PP::ProviderUseProxyType  enType;
			VARIANT_BOOL bInit;
			_bstr_t bsValue;
			spProviderData->get_NeedLogin(&bInit);
			if(bInit!=VARIANT_FALSE)
			{
				spProviderData->UseProxy =(PP::ProviderUseProxyType)opts.m_PriceProvOpts.m_iUseProxy;
				spProviderData->UserAccount = _bstr_t((LPCTSTR)opts.m_PriceProvOpts.m_strAccount);
				spProviderData->UserPassword= _bstr_t((LPCTSTR)opts.m_PriceProvOpts.m_strPassword);
				spProviderData->ProxyName =  _bstr_t((LPCTSTR)opts.m_PriceProvOpts.m_strProxy);;
				spProviderData->ProxyPort = opts.m_PriceProvOpts.m_iProxyPort;
			}
		}

/*
		keyMyTrack.SetValue (opts.m_PriceProvOpts.m_strAccount, _T("Account"));
		keyMyTrack.SetValue (opts.m_PriceProvOpts.m_strPassword, _T("Password"));
		keyMyTrack.SetValue ( (DWORD) 1 , _T("RevHi"));
		keyMyTrack.SetValue ( (DWORD) 0 , _T("RevLo"));
		keyMyTrack.SetValue (_T("MyTrackPrvs"), _T("ProductName"));
		keyMyTrack.SetValue (opts.m_PriceProvOpts.m_iUseProxy, _T("UseProxy"));
		keyMyTrack.SetValue (opts.m_PriceProvOpts.m_strProxy, _T("Proxy"));
		keyMyTrack.SetValue (opts.m_PriceProvOpts.m_iProxyPort, _T("Port"));		
*/
	}

	m_opts = opts;

	if(FAILED(XMLParams.SaveXMLParams()))
		return false;

	return true;
}


CVolaNotificationClient * CIVAnalyzerApp::GetVolaNotificationClient ()
{
	ASSERT(m_pMainWnd);
	return  STATIC_DOWNCAST(CIVAnalyzerBar, m_pMainWnd)->GetVolaNotificationClient();
}



bool CIVAnalyzerApp::CheckLicense(LPCTSTR szKey)
{
	CLicenseKey keyToCheck = szKey;

	if ( keyToCheck  == m_license )
	{
		m_bExpired = keyToCheck.IsExpired ();
		if (! m_bExpired)
		{
			m_license = keyToCheck;
			return true;
		}
	}

	return false;
}



#define CHAR64(c) (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])
static char basis_64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????";
static char index_64[128] = {
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

int encode64(const char *_in, unsigned inlen,
char *_out, unsigned outmax, unsigned *outlen)
{
	const unsigned char *in = (const unsigned char *) _in;
	unsigned char *out = (unsigned char *) _out;
	unsigned char oval;
	char *blah;
	unsigned olen;
	
	olen = (inlen + 2) / 3 * 4;
	if (outlen)
		*outlen = olen;
	if (outmax < olen)
		return -2;
	
	blah = (char *) out;
	while (inlen >= 3)
	{
		/* user provided max buffer size; make sure we don't go over it */
		*out++ = basis_64[in[0] >> 2];
		*out++ = basis_64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = basis_64[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = basis_64[in[2] & 0x3f];
		in += 3;
		inlen -= 3;
	}
	if (inlen > 0)
	{
	/* user provided max buffer size; make sure we don't go over it */
		*out++ = basis_64[in[0] >> 2];
		oval = (in[0] << 4) & 0x30;
		if (inlen > 1)
			oval |= in[1] >> 4;
		*out++ = basis_64[oval];
		*out++ = (inlen < 2) ? '=' : basis_64[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	if (olen < outmax)
		*out = '\0';

	return 0;
}

int decode64(const char *in, unsigned inlen, char *out, unsigned *outlen)
{
	unsigned len = 0, lup;
	int c1, c2, c3, c4;

	if (in[0] == '+' && in[1] == ' ')
		in += 2;

	if (*in == '\0')
		return -1;

	for (lup = 0; lup < inlen / 4; lup++)
	{
		c1 = in[0];
		if (CHAR64(c1) == -1)
			return -1;
		c2 = in[1];
		if (CHAR64(c2) == -1)
			return -1;
		c3 = in[2];
		if (c3 != '=' && CHAR64(c3) == -1)
			return -1;
		c4 = in[3];
		if (c4 != '=' && CHAR64(c4) == -1)
			return -1;
		in += 4;
		*out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
		++len;
		if (c3 != '=')
		{
			*out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);
			++len;
			if (c4 != '=')
			{
				*out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);
				++len;
			}
		}
	}
	*out = 0;
	*outlen = len;

return 0;
}