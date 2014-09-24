// EtsEodManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "EtsEodManagerDoc.h"
#include "EtsEodManagerView.h"
#include "ModuleVersionEx.h"
#include "EodCommandLineInfo.h"
#include "EgDebugUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEtsEodManagerApp

CAtlMfcModule _AtlModule;

BEGIN_MESSAGE_MAP(CEtsEodManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()


// CEtsEodManagerApp construction

CEtsEodManagerApp::CEtsEodManagerApp() : m_bExitNow(false)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CEtsEodManagerApp object

CEtsEodManagerApp g_App;

// CEtsEodManagerApp initialization
BOOL CEtsEodManagerApp::InitInstance()
{
	if(!CEtsEodManagerApp::_IsFirstInstance())
		return FALSE;

	SetMiniDumpDefaultCrashHandler();

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
/*#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = ::CoInitialize(NULL);
#endif
	if(FAILED(hRes))
		return FALSE;*/

	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Egar\\ETS"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEtsEodManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEtsEodManagerView));
	AddDocTemplate(pDocTemplate);

	bool bPathInitialized = _InitReportsPath();

	// Parse command line for standard shell commands, DDE, file open
	CEodCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	

	_InitTypeIcons();

    m_nCmdShow = SW_HIDE; // Hide while creation

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
		cmdInfo.m_strFileName.Empty();
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// The one and only window has been initialized, so show and update it
	PersistWindowPlacement(m_pMainWnd, _T("MainWindow"), false, cmdInfo.m_bIsBackground);
	
	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(m_pMainWnd);
	if(pMainFrame)
		pMainFrame->OnFirstActivate();
	
	m_pMainWnd->UpdateWindow();

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	long nVal = TRUE;
	XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\Settings"), _T("FirstStart"), &nVal, TRUE);
	BOOL bFirstStart = nVal;

	if(bFirstStart)
	{
		XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\Settings"), _T("FirstStart"), FALSE);
		XMLParams.SaveXMLParams();

		m_pMainWnd->PostMessage(WM_COMMAND, ID_OPTIONS_CONNECTIONS, 0);
	}

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

bool CEtsEodManagerApp::_InitReportsPath()
{
	TCHAR szAppPath[4096] = {0};

	::GetModuleFileName(m_hInstance, szAppPath, 4096);
	m_strReportsPath = CModuleVersionEx::GetFilePath(CModuleVersionEx::GetLongPathName(szAppPath));
	m_strReportsPath += "Logs\\";

	if(!::CreateDirectory(m_strReportsPath, NULL))
	{
		if(ERROR_ALREADY_EXISTS != ::GetLastError())
			return false;
	}

	m_strCurrentReportFilePath = m_strReportsPath;
	m_strCurrentReportFilePath += _T("EodReport.EtsEodRpt");

	if(!::PathFileExists(m_strCurrentReportFilePath))
	{
		CFile file;
		if(!file.Open(m_strCurrentReportFilePath, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate))
			return false;
		file.Close();
	}
	return true;
}

// App command to run the dialog
void CEtsEodManagerApp::OnAppAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CEtsEodManagerApp::PersistWindowPlacement(CWnd* pWnd, LPCTSTR lpszWindowName, bool bSave, bool bIsBackground)
{
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CWnd, pWnd);

	if(!pWnd || !::IsWindow(pWnd->m_hWnd) || !lpszWindowName)
		return;

	WINDOWPLACEMENT wndpl = {0};
	wndpl.length = sizeof(WINDOWPLACEMENT);

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	if(bSave)
	{
		if(GetWindowPlacement(pWnd->m_hWnd, &wndpl))
		{
			XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionLeft"), wndpl.rcNormalPosition.left);
			XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionTop"), wndpl.rcNormalPosition.top);
			XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionRight"), wndpl.rcNormalPosition.right);
			XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionBottom"), wndpl.rcNormalPosition.bottom);
			XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("ShowCmd"), wndpl.showCmd);

			XMLParams.SaveXMLParams();
		}
	}
	else
	{
		XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionLeft"), &wndpl.rcNormalPosition.left, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionTop"), &wndpl.rcNormalPosition.top, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionRight"), &wndpl.rcNormalPosition.right, 0xffff);
		XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("NormalPositionBottom"), &wndpl.rcNormalPosition.bottom, 0xffff);

		long nVal = 0xffff;
		XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\MainWindow"), _T("ShowCmd"), &nVal, 0xffff);
		wndpl.showCmd = nVal;

		if(!bIsBackground)
		{
			if(0xffff == wndpl.rcNormalPosition.left
				|| 0xffff == wndpl.rcNormalPosition.top || 0xffff == wndpl.rcNormalPosition.right
				|| 0xffff == wndpl.rcNormalPosition.bottom || 0xffff == wndpl.showCmd)
			{
				pWnd->ShowWindow(SW_SHOW);
			}
			else
			{
				if(SW_SHOWMINIMIZED == wndpl.showCmd || SW_SHOWMINNOACTIVE == wndpl.showCmd)
					wndpl.showCmd = SW_SHOWNORMAL;

				SetWindowPlacement(pWnd->m_hWnd, &wndpl);
			}
		}
		else
			pWnd->ShowWindow(SW_HIDE);
	}
}

bool CEtsEodManagerApp::_IsFirstInstance()
{
	CWnd* pWndPrev = CWnd::FindWindow(g_cszMainFrameClassName, NULL);

	if(pWndPrev)
	{
		CWnd* pWndChild = pWndPrev->GetLastActivePopup();

		if(pWndPrev->IsIconic())
			pWndPrev->ShowWindow(SW_RESTORE);
		else if(!pWndPrev->IsWindowVisible())
			pWndPrev->ShowWindow(SW_SHOW);

		pWndChild->SetForegroundWindow();
		return false;
	}

	return true;
}

void CEtsEodManagerApp::OnAppExit()
{
	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(m_pMainWnd);
	if(pMainFrame)
	{
		if(pMainFrame->IsTimerSet())
		{
			int nResult = pMainFrame->MessageBox(_T("If you exit application, Eod server will not run by timer. \nContinue anyway?"), NULL, MB_ICONWARNING | MB_YESNO);
			if(nResult != IDYES)
				return;
		}
	}
	m_bExitNow = true;
	__super::OnAppExit();
}

void CEtsEodManagerApp::_InitTypeIcons()
{
	_ClearTypeIcons();

	_LoadTypeIcons(IDI_TYPE_START);
	_LoadTypeIcons(IDI_TYPE_INFO);
	_LoadTypeIcons(IDI_TYPE_WARNING);
	_LoadTypeIcons(IDI_TYPE_ERROR);
	_LoadTypeIcons(IDI_TYPE_END);
	_LoadTypeIcons(IDI_TYPE_END_WITH_ERRORS);
}

void CEtsEodManagerApp::_LoadTypeIcons(long nResID)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
							MAKEINTRESOURCE(nResID), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	
	_variant_t vtIcon;

	if(hIcon)
	{
		CPictureHolder pic;
		if(pic.CreateFromIcon(hIcon))
		{
			LPPICTUREDISP lppicDisp = pic.GetPictureDispatch();
			if(lppicDisp)
				vtIcon = _variant_t(lppicDisp, false);
		}

		::DestroyIcon(hIcon);
	}

	m_mapTypeIcons[nResID] = vtIcon;
}

void CEtsEodManagerApp::_ClearTypeIcons()
{
	m_mapTypeIcons.clear();
}

_variant_t CEtsEodManagerApp::GetTypeIcon(long nResID)
{
	CIconByIdMap::iterator itPic = m_mapTypeIcons.find(nResID);
	if(itPic != m_mapTypeIcons.end())
		return itPic->second;
	else
		return _variant_t();
}
