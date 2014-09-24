// EtsFixManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "EtsFixManagerDoc.h"
#include "EtsFixManagerView.h"
#include "ModuleVersionEx.h"
#include "FixCommandLineInfo.h"
#include "EgDebugUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEtsFixManagerApp

BEGIN_MESSAGE_MAP(CEtsFixManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()


// CEtsFixManagerApp construction

CEtsFixManagerApp::CEtsFixManagerApp() : m_bExitNow(false)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CEtsFixManagerApp object

CEtsFixManagerApp g_App;

// CEtsFixManagerApp initialization

BOOL CEtsFixManagerApp::InitInstance()
{

	if(!CEtsFixManagerApp::_IsFirstInstance())
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
		RUNTIME_CLASS(CEtsFixManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEtsFixManagerView));
	AddDocTemplate(pDocTemplate);
	
	// Parse command line for standard shell commands, DDE, file open
	CFixCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	_InitTypeIcons();

	m_nCmdShow = SW_HIDE; // Hide while creation
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	PersistWindowPlacement(m_pMainWnd, _T("MainWindow"), false, cmdInfo.m_bIsBackground);

	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(m_pMainWnd);
	if(pMainFrame)
		pMainFrame->OnFirstActivate();

	m_pMainWnd->UpdateWindow();

	BOOL bFirstStart = GetProfileInt(_T("Settings"), _T("FirstStart"), TRUE);
	if(bFirstStart)
	{
		WriteProfileInt(_T("Settings"), _T("FirstStart"), FALSE);
		m_pMainWnd->PostMessage(WM_COMMAND, ID_OPTIONS_CONNECTIONS, 0);
	}
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// App command to run the dialog
void CEtsFixManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CEtsFixManagerApp::PersistWindowPlacement(CWnd* pWnd, LPCTSTR lpszWindowName, bool bSave, bool bIsBackground)
{
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CWnd, pWnd);

	if(!pWnd || !::IsWindow(pWnd->m_hWnd) || !lpszWindowName)
		return;

	WINDOWPLACEMENT wndpl = {0};
	wndpl.length = sizeof(WINDOWPLACEMENT);

	if(bSave)
	{
		if(GetWindowPlacement(pWnd->m_hWnd, &wndpl))
		{
			WriteProfileInt(lpszWindowName, _T("NormalPositionLeft"), wndpl.rcNormalPosition.left);
			WriteProfileInt(lpszWindowName, _T("NormalPositionTop"), wndpl.rcNormalPosition.top);
			WriteProfileInt(lpszWindowName, _T("NormalPositionRight"), wndpl.rcNormalPosition.right);
			WriteProfileInt(lpszWindowName, _T("NormalPositionBottom"), wndpl.rcNormalPosition.bottom);
			WriteProfileInt(lpszWindowName, _T("ShowCmd"), wndpl.showCmd);
		}
	}
	else
	{
		wndpl.rcNormalPosition.left = GetProfileInt(lpszWindowName, _T("NormalPositionLeft"), 0xffff);
		wndpl.rcNormalPosition.top = GetProfileInt(lpszWindowName, _T("NormalPositionTop"), 0xffff);
		wndpl.rcNormalPosition.right =	GetProfileInt(lpszWindowName, _T("NormalPositionRight"), 0xffff);
		wndpl.rcNormalPosition.bottom = GetProfileInt(lpszWindowName, _T("NormalPositionBottom"), 0xffff);
		wndpl.showCmd = GetProfileInt(lpszWindowName, _T("ShowCmd"), 0xffff);

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

bool CEtsFixManagerApp::_IsFirstInstance()
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

void CEtsFixManagerApp::OnAppExit()
{
	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(m_pMainWnd);
	if(pMainFrame)
	{
		int nResult = pMainFrame->MessageBox(_T("If you exit application, FIX Gateway will be stopped. \nContinue anyway?"), NULL, MB_ICONWARNING | MB_YESNO);
		if(nResult != IDYES)
			return;
	}
	m_bExitNow = true;
	__super::OnAppExit();
}

void CEtsFixManagerApp::_InitTypeIcons()
{
	_ClearTypeIcons();

	_LoadTypeIcons(IDI_TYPE_START);
	_LoadTypeIcons(IDI_TYPE_INFO);
	_LoadTypeIcons(IDI_TYPE_WARNING);
	_LoadTypeIcons(IDI_TYPE_ERROR);
	_LoadTypeIcons(IDI_TYPE_END);
}

void CEtsFixManagerApp::_LoadTypeIcons(long nResID)
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

void CEtsFixManagerApp::_ClearTypeIcons()
{
	m_mapTypeIcons.clear();
}

_variant_t CEtsFixManagerApp::GetTypeIcon(long nResID)
{
	CIconByIdMap::iterator itPic = m_mapTypeIcons.find(nResID);
	if(itPic != m_mapTypeIcons.end())
		return itPic->second;
	else
		return _variant_t();
}