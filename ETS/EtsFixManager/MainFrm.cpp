// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "FixSettingsDlg.h"
#include "ConnectionsDlg.h"
#include "MainFrm.h"
#include "EtsFixManagerView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

_ATL_FUNC_INFO CMainFrame::m_OnLogMessage = 
{CC_STDCALL, VT_EMPTY, 3, {VT_DATE, VT_I4, VT_BSTR} };

_ATL_FUNC_INFO CMainFrame::m_OnStatusChanged = 
{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskbarCreated)
	ON_COMMAND(ID_OPTIONS_SETTINGS, OnOptionsSettings)
	ON_COMMAND(ID_OPTIONS_CONNECTIONS, OnOptionsConnections)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_START_FIX, OnFileStartFix)
	ON_UPDATE_COMMAND_UI(ID_FILE_START_FIX, OnUpdateFileStartFix)
	ON_COMMAND(ID_FILE_STOP_FIX, OnFileStopFix)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP_FIX, OnUpdateFileStopFix)
	ON_COMMAND(ID_EDIT_COPY, OnCopyGrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCopyGrid)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN, OnOpen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_enFixStatus(enFgSsStopped), m_bFixAutoStartStop(false)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_wndToolBar.CreateEx(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(!m_wndReBar.Create(this) || 
		!m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_NOGRIPPER | RBBS_FIXEDBMP))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	if(!m_wndTray.Create(NULL, WM_ICON_NOTIFY, _T(""), NULL, IDR_POPUPMENU))
		return -1;

	m_wndTray.SetIcon(IDI_STOP);
	m_wndTray.SetTooltipText(AFX_IDS_APP_TITLE);

	_InitFixGServer();

	return 0;
}
void CMainFrame::_InitFixGServer()
{
	try
	{
		CWaitCursor wait;

		__CHECK_HRESULT(m_spFixGSrv.CreateInstance(__uuidof(FixGServer)),
			_T("Fail to create Fix Gateway Server object."));


		__CHECK_HRESULT(DispEventAdvise(m_spFixGSrv),
			_T("Fail to connect to Fix Gateway Server events."));

		m_dtFixStartTime = m_spFixGSrv->StartTime;
		m_dtFixStopTime = m_spFixGSrv->StopTime;
		m_bFixAutoStartStop = m_spFixGSrv->AutoStartStop ? true : false;	
	}
	catch(_com_error& e)
	{
		m_spFixGSrv = NULL;

		CString strError;
		strError.Format(_T("Fail to initialize Fix Gateway server: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{ 
	WNDCLASS wndcls = { 0 };
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszClassName = g_cszMainFrameClassName;
	wndcls.hIcon = g_App.LoadIcon(IDR_MAINFRAME);

	if(!AfxRegisterClass(&wndcls))
		return FALSE;

	cs.lpszClass = g_cszMainFrameClassName;
	cs.style = WS_OVERLAPPEDWINDOW ;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

// CMainFrame message handlers
void CMainFrame::OnOptionsSettings()
{
	if(m_spFixGSrv == NULL)
	{
		MessageBox(_T("Fail to setup settings. FIX Gateway server is not properly initialized."), NULL, MB_ICONSTOP);
		return;
	}

	CFixSettingsDlg dlg;

	dlg.m_dtStartTime = m_dtFixStartTime;
	dlg.m_dtStopTime = m_dtFixStopTime;
	dlg.m_bAutoStartStop = m_bFixAutoStartStop;

	if(dlg.DoModal() == IDOK)
	{
		try
		{
			CWaitCursor wait;
			
			m_dtFixStartTime = dlg.m_dtStartTime;
			m_dtFixStopTime = dlg.m_dtStopTime;
			m_bFixAutoStartStop = dlg.m_bAutoStartStop;
			m_spFixGSrv->StartTime = m_dtFixStartTime;
			m_spFixGSrv->StopTime = m_dtFixStopTime;
			m_spFixGSrv->AutoStartStop = m_bFixAutoStartStop ? VARIANT_TRUE : VARIANT_FALSE;
		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Fail to apply FIX Gateway server settings: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CMainFrame::OnOptionsConnections()
{
	CConnectionsDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bIsDbChanged)
			MessageBox(_T("New settings will be actual on next start of FIX Gateway server."),
			NULL, MB_ICONEXCLAMATION);
	}
}

void CMainFrame::OnDestroy()
{
	try
	{
		if(m_spFixGSrv != NULL)
		{
			DispEventUnadvise(m_spFixGSrv);
			m_spFixGSrv->StopFix();
			m_spFixGSrv = NULL;
		}
	}
	catch (_com_error& e)
	{
		CString strError;
		strError.Format(_T("Fail to uninitialize FIX Gateway Server: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

	g_App.PersistWindowPlacement(this, _T("MainWindow"), true);
	CFrameWnd::OnDestroy();
}

void CMainFrame::OnFirstActivate()
{
	CEtsFixManagerView* pView = reinterpret_cast<CEtsFixManagerView*>(GetActiveView());
	if(pView)
		pView->AdjustLastColWidth();
}

void CMainFrame::OnFileStartFix()
{
	if(m_spFixGSrv == NULL)
		MessageBox(_T("Fix Gateway server is not properly initialized."), NULL, MB_ICONSTOP);
	else
	{
		try
		{
			CWaitCursor wait;

			__CHECK_HRESULT(m_spFixGSrv->StartFix(), _T(""));

			m_enFixStatus = m_spFixGSrv->Status;
		}
		catch(_com_error& e)
		{
			m_enFixStatus = m_spFixGSrv->Status;

			CString strError;
			strError.Format(_T("Fail to start Fix Gateway server: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CMainFrame::OnUpdateFileStartFix(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->Enable(enFgSsStopped == m_enFixStatus);
}

void CMainFrame::OnFileStopFix()
{
	if(m_spFixGSrv == NULL)
		MessageBox(_T("Fix Gateway server is not properly initialized."), NULL, MB_ICONSTOP);
	else
	{
		try
		{
			CWaitCursor wait;
			__CHECK_HRESULT(m_spFixGSrv->StopFix(), _T(""));

			m_enFixStatus = m_spFixGSrv->Status;
		}
		catch(_com_error& e)
		{
			m_enFixStatus = m_spFixGSrv->Status;
			CString strError;
			strError.Format(_T("Fail to stop Fix Gateway server: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CMainFrame::OnUpdateFileStopFix(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->Enable(enFgSsStarted == m_enFixStatus);
}

STDMETHODIMP CMainFrame::OnLogMessage(DATE dtTime, FgLogMessageTypeEnum enType, BSTR bsMessage)
{
	CEtsFixManagerDoc* pDoc = static_cast<CEtsFixManagerDoc*>(GetActiveDocument());
	if(pDoc)
		pDoc->AddNewRecord(dtTime, enType, bsMessage);
	return S_OK;
}

STDMETHODIMP CMainFrame::OnStatusChanged(FgServerStatusEnum enStatus)
{
	m_enFixStatus = enStatus;
	
	if(enFgSsStarted == m_enFixStatus)
		m_wndTray.SetIcon(IDI_START);
	else
		m_wndTray.SetIcon(IDI_STOP);

	return S_OK;
}

LRESULT CMainFrame::OnTaskbarCreated(WPARAM wp, LPARAM lp)
{
	m_wndTray.AddIcon();
	return 0;
}

void CMainFrame::OnClose()
{
	if(!g_App.m_bExitNow)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		__super::OnClose();
	}
}

void CMainFrame::OnOpen()
{
	if(IsIconic())
		ShowWindow(SW_RESTORE);
	else
		ShowWindow(SW_SHOW);
}	

void CMainFrame::OnCopyGrid()
{
	CEtsFixManagerDoc* pDoc = static_cast<CEtsFixManagerDoc*>(GetActiveDocument());
	if(pDoc)
		pDoc->CopyLogToClipboard();
}

void CMainFrame::OnUpdateCopyGrid(CCmdUI *pCmdUI)
{
	CEtsFixManagerDoc* pDoc = static_cast<CEtsFixManagerDoc*>(GetActiveDocument());
	if(pCmdUI && pDoc)
		pCmdUI->Enable(pDoc->GetRecordsCount());
}