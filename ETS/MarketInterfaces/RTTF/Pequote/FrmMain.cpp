//**************************************************************************************************************//
// solution:	Pequote
// project:		Pequote
// filename:	FrmMain.cpp
// created:		27-Dec-2002 17:09 by Suchkov Dmitry
//
// purpose:		implementation of CFrmMain
//
//**************************************************************************************************************//

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "stdafx.h"
#include "Pequote.h"

#include "FrmMain.h"
#include "DlgSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**************************************************************************************************************//
// variables
//**************************************************************************************************************//
extern CPequoteApp theApp;

//**************************************************************************************************************//
// constants
//**************************************************************************************************************//
extern LPCTSTR c_szSingleInstanceId;

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define REG_FRMMAIN _T("main")

//--------------------------------------------------------------------------------------------------------------//
static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//--------------------------------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CFrmMain, CFrameWnd)

BEGIN_MESSAGE_MAP(CFrmMain, CFrameWnd)
	//{{AFX_MSG_MAP(CFrmMain)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_INSERT_TRACE_MESSAGE, OnInsertTraceMessage)
	ON_COMMAND(ID_ACTION_START, OnActionStart)
	ON_UPDATE_COMMAND_UI(ID_ACTION_START, OnUpdateActionStart)
	ON_COMMAND(ID_ACTION_STOP, OnActionStop)
	ON_UPDATE_COMMAND_UI(ID_ACTION_STOP, OnUpdateActionStop)
	ON_COMMAND(ID_ACTION_SETTINGS, OnOptionsSetting)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------//
CFrmMain::CFrmMain()
:	m_hThread(NULL)
,   m_bAutoStart(false)
{
}

//--------------------------------------------------------------------------------------------------------------//
CFrmMain::~CFrmMain()
{
	CThread::Stop();
}

//--------------------------------------------------------------------------------------------------------------//
UINT CFrmMain::Run()
{
	HANDLE hEvents[] =
	{
		m_hCloseApplication,
			CThread::GetStopEventHandle()
	}  ;

	DWORD dw = WaitForMultipleObjects(2,hEvents,FALSE, INFINITE);
	if(WAIT_OBJECT_0 == dw)
	{
		OnActionStop();
		//theApp.m_pMainWnd->PostMessage(ID_APP_EXIT);
		//theApp.m_pMainWnd->PostMessage(WM_DESTROY);
		PostMessage(WM_CLOSE);
	}
	return 0;
}

//--------------------------------------------------------------------------------------------------------------//
int CFrmMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_hCloseApplication.Attach(::CreateEvent(NULL, TRUE, FALSE, theApp.GetStopEventName().c_str()));
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndView.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (NULL != hIcon)
	{
		SetIcon(hIcon, TRUE);
		SetIcon(hIcon, FALSE);
		::DeleteObject(hIcon);
	}
	else
	{
		TRACE0("Failed to load icon\n");
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	SetTimer(1, 50,NULL);

	WindowPlacementLoad();
	if(m_bAutoStart)
		OnActionStart();

	CThread::Start();
	return 0;
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CFrmMain::PreCreateWindow(CREATESTRUCT& cs)
{
	if (FALSE == CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	lstrcpyn(AfxGetThreadState()->m_szTempClassName, c_szSingleInstanceId, 
		sizeof(AfxGetThreadState()->m_szTempClassName) / sizeof(TCHAR));

	cs.lpszClass = AfxGetThreadState()->m_szTempClassName;
	
	WNDCLASS wndcls = {0};
	HINSTANCE hInst = NULL;
	hInst = AfxGetInstanceHandle();
	if (FALSE == ::GetClassInfo(hInst, cs.lpszClass, &wndcls))
	{
		wndcls.style = 0;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = NULL;
		wndcls.hbrBackground = NULL;
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = cs.lpszClass;
		if (!AfxRegisterClass(&wndcls)) AfxThrowResourceException();	
	}
	else
	{
		ASSERT(wndcls.style == 0);
		return TRUE;
	}
	
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CFrmMain::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CFrmMain::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnSetFocus(CWnd* pOldWnd)
{
	m_wndView.SetFocus();
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CFrmMain::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//--------------------------------------------------------------------------------------------------------------//
LRESULT CFrmMain::OnInsertTraceMessage(WPARAM wParam, LPARAM lParam)
{
	if (NULL == lParam) return 0;
	
	m_wndView.InsertTraceMessage(
		static_cast<CTracer::MessageTypeEnum>(wParam),
		reinterpret_cast<sMessage*>(lParam));

	return 0;
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnActionStart() 
{
	if (m_hThread) 
		::CloseHandle(m_hThread);
	
	UINT nID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, PequoteProc, this, 0, &nID);
	if (!m_hThread)
	{
		CTracer::TraceMessage(CTracer::enMtError,NULL, _T("Failed to create thread."));	
	}
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnUpdateActionStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsRunning() == FALSE);
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnActionStop() 
{
	if (CPequote::m_hStopApplication)
		::SetEvent(CPequote::m_hStopApplication);
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnUpdateActionStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE == IsRunning());
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnOptionsSetting() 
{
	CDlgSettings dlgSettings(m_strSessionID);	
	dlgSettings.DoModal();
}

//--------------------------------------------------------------------------------------------------------------//
bool CFrmMain::IsRunning() const
{
	DWORD dwExitCode;
	return (m_hThread && ::GetExitCodeThread(m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE);
}

//--------------------------------------------------------------------------------------------------------------//
UINT __stdcall CFrmMain::PequoteProc(PVOID pData)
{
	CFrmMain* pMain = reinterpret_cast<CFrmMain*>(pData);
	CPequote* m_pPequote = new CPequote(pMain->GetSessionID());

	DWORD dwResult = ERROR_SUCCESS;
	if (m_pPequote)
	{
		dwResult = m_pPequote->Start();
		if (dwResult)
	    {
		    CTracer::TraceMessage(CTracer::enMtError,NULL, _T("Failed to start application."));
	    }
	    else
	    {
		    dwResult = m_pPequote->Run();
		    if (dwResult)
		    {
			    CTracer::TraceMessage(CTracer::enMtError,NULL, _T("Failed to run application."));
		    }
		    else
		    {
			    dwResult = m_pPequote->Stop();
			    if (dwResult)
			    {
				    CTracer::TraceMessage(CTracer::enMtError,NULL, _T("While stopping some errors occured."));
			    }
		    }
	    }
	}
	else
	{
		CTracer::TraceMessage(CTracer::enMtError,NULL, _T("Internal error. Not enough memory."));
	}

	if (m_pPequote)
		delete m_pPequote;

	return 0;
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnEditClear() 
{
	m_wndView.DeleteAllItems();	
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndView.GetItemCount());
}

//--------------------------------------------------------------------------------------------------------------//
CString CFrmMain::MakeItemString(int nItem)
{
	return (m_wndView.GetItemText(nItem, 0) + "\t" +
					m_wndView.GetItemText(nItem, 1) + "\t" +
					m_wndView.GetItemText(nItem, 2) + "\t" +
					m_wndView.GetItemText(nItem, 3) + "\r\n");
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnEditCopy() 
{
	CString sLine("");

	POSITION pos = m_wndView.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		for(int i = 0; i < m_wndView.GetItemCount(); i++) sLine += MakeItemString(i);
	}
	else
	{
		int nItem;
		while (pos)
		{
			nItem = m_wndView.GetNextSelectedItem(pos);
			sLine += MakeItemString(nItem);
		}
	}
	if (::OpenClipboard(m_wndView) && EmptyClipboard())
	{
		HGLOBAL p = ::GlobalAlloc(GMEM_MOVEABLE, (sLine.GetLength() + 1) * sizeof(TCHAR));
		if (p)
		{
			LPSTR psz = (LPSTR)::GlobalLock(p);
			memcpy(psz, (LPCTSTR)sLine, (sLine.GetLength() + 1) * sizeof(TCHAR));
			GlobalUnlock(p);
			SetClipboardData(CF_OEMTEXT, p);
		}
		CloseClipboard();
	}
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndView.GetItemCount());
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::OnClose() 
{
	OnActionStop();

	if (m_hThread)
    { 
		
		while (WAIT_OBJECT_0 != ::MsgWaitForMultipleObjects(1, &m_hThread,
            FALSE, INFINITE, QS_SENDMESSAGE))
        {
			MSG msg = {0};
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
                if (msg.message != WM_CLOSE)
                {
	    			TranslateMessage(&msg);
    				DispatchMessage(&msg);
                }
			}
        }

		::CloseHandle(m_hThread);
        m_hThread = NULL;
    }

	WindowPlacementSave();

	CFrameWnd::OnClose();
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::WindowPlacementSave()
{
	WINDOWPLACEMENT wndpl;
	if (GetWindowPlacement(&wndpl))
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("Flags"), wndpl.flags);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MaxPositionX"), wndpl.ptMaxPosition.x);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MaxPositionY"), wndpl.ptMaxPosition.y);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MinPositionX"), wndpl.ptMinPosition.x);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MinPositionY"), wndpl.ptMinPosition.y);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionLeft"), wndpl.rcNormalPosition.left);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionTop"), wndpl.rcNormalPosition.top);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionRight"), wndpl.rcNormalPosition.right);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionBottom"), wndpl.rcNormalPosition.bottom);
		XMLParams.SetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("ShowCmd"), wndpl.showCmd);

		XMLParams.SaveXMLParams();
	}
}

//--------------------------------------------------------------------------------------------------------------//
void CFrmMain::WindowPlacementLoad()
{
	WINDOWPLACEMENT wndpl;

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MaxPositionX"), &wndpl.ptMaxPosition.x, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MaxPositionY"), &wndpl.ptMaxPosition.y, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MinPositionX"), &wndpl.ptMinPosition.x, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("MinPositionY"), &wndpl.ptMinPosition.y, 0xffff);

	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionLeft"), &wndpl.rcNormalPosition.left, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionTop"), &wndpl.rcNormalPosition.top, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionRight"), &wndpl.rcNormalPosition.right, 0xffff);
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("NormalPositionBottom"), &wndpl.rcNormalPosition.bottom, 0xffff);

	long nVal = 0xffff;
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("Flags"), &nVal, 0xffff);
	wndpl.flags = nVal;
	XMLParams.GetXMLLong(SETTINGS_WINDOW_PLACEMENT_KEY, _T("ShowCmd"), &nVal, 0xffff);
	wndpl.showCmd = nVal;

	if(0xffff == wndpl.rcNormalPosition.left
		|| 0xffff == wndpl.rcNormalPosition.top || 0xffff == wndpl.rcNormalPosition.right
		|| 0xffff == wndpl.rcNormalPosition.bottom || 0xffff == wndpl.showCmd)
	{
		return;
	}

	SetWindowPlacement(&wndpl);
}