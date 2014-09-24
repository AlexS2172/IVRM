// CIBCTradeFeedAdapter.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CIBCTradeFeedAdapter.h"

#include "Settings.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp

//**************************************************************************************************************//
// constants
//**************************************************************************************************************//
LPCTSTR c_szSingleInstanceId = _T("EGAR CIBCADAPTER {E39D90DA-EDEA-4473-A87F-121C5C66AEF0}");


BEGIN_MESSAGE_MAP(CTFAdapterApp, CWinApp)
	//{{AFX_MSG_MAP(CTFAdapterApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

DWORD CheckLogsFolder()
{
	_TCHAR szBuf[_MAX_PATH];
	DWORD dwErr = GetModuleFileName(NULL, szBuf, _MAX_PATH);
	if(dwErr == 0)
		return GetLastError();

	_TCHAR szDrive[_MAX_DRIVE];
	_TCHAR szDir[_MAX_DIR];
	_TCHAR szFname[_MAX_FNAME];
	_TCHAR szExt[_MAX_EXT];
	_tsplitpath( szBuf, szDrive, szDir, szFname, szExt );

	_TCHAR szLogsPath[_MAX_PATH];
	_TCHAR szCurrentPath[_MAX_PATH];
	_stprintf(szLogsPath, _T("%s%sLogs"), szDrive, szDir);
	_stprintf(szCurrentPath, _T("%s%s"), szDrive, szDir);

	BOOL bRes = CreateDirectory(szLogsPath, NULL);
	if(!bRes)
	{
		dwErr = GetLastError();
		if(ERROR_ALREADY_EXISTS != dwErr)
			return dwErr;
	}

	bRes = SetCurrentDirectory(szCurrentPath);
	if(!bRes)
		return GetLastError();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp construction

CTFAdapterApp::CTFAdapterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CTFAdapterApp::~CTFAdapterApp()
{
	CoUninitialize();
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CTFAdapterApp object

CTFAdapterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp initialization

BOOL CTFAdapterApp::InitInstance()
{
	DWORD dwRes = CheckLogsFolder();

	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, c_szSingleInstanceId);
	if (NULL != hMutex)
	{
		HWND hWnd = ::FindWindowEx(NULL, NULL, NULL, AfxGetApp()->m_pszAppName);
		if (NULL != hWnd) 
			::SetForegroundWindow(hWnd);
		
		return FALSE;
	}
	else
	{
		m_hSingleInstanceMutex = ::CreateMutex(NULL, TRUE, c_szSingleInstanceId);
	}
    
//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	SetRegistryKey(_T("Egar\\ETS"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	CTracer::SetWindowHandle(pFrame->GetSafeHwnd());
	CTracer::SetMessageId(WM_INSERT_TRACE_MESSAGE);

    CSettings::SetDefaults();

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTFAdapterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp message handlers

