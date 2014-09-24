// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "EodSettingsDlg.h"
#include "ConnectionsDlg.h"
#include "MainFrm.h"
#include "EtsEodManagerView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

_ATL_FUNC_INFO CMainFrame::m_OnEodStatusChangedInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };

_ATL_FUNC_INFO CMainFrame::m_OnEodErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskbarCreated)
	ON_COMMAND(ID_OPTIONS_SETTINGS, OnOptionsSettings)
	ON_COMMAND(ID_OPTIONS_CONNECTIONS, OnOptionsConnections)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_START_EOD, OnFileStartEod)
	ON_UPDATE_COMMAND_UI(ID_FILE_START_EOD, OnUpdateFileStartEod)
	ON_COMMAND(ID_FILE_STOP_EOD, OnFileStopEod)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP_EOD, OnUpdateFileStopEod)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN, OnOpen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_nEodSteps(enSmDefSteps),
	m_bEodRunPeriodically(false),
	m_dtEodStartTime(0, 0, 0, 6, 0, 0),
	m_enCalcModel(enEodCmBinomial),
	m_nLogHistory(0),
	m_nLogLevel(0),
	m_nEodState(enEodStateUnavailable)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
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

	if(!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	if(!m_wndTray.Create(NULL, WM_ICON_NOTIFY, _T(""), NULL, IDR_POPUPMENU))
		return -1;
	
	m_wndTray.SetIcon(IDR_MAINFRAME);
	m_wndTray.SetTooltipText(AFX_IDS_APP_TITLE);

	_InitEodServer();

	return 0;
}

void CMainFrame::_InitEodServer()
{
	try
	{
		CWaitCursor wait;

		__CHECK_HRESULT(m_spEodSrv.CreateInstance(CLSID_EodServer),
						_T("Fail to create End of Day Server object."));

		m_nEodSteps = m_spEodSrv->EodMask;
		m_enCalcModel = m_spEodSrv->EodCalcModel;
		m_nLogLevel = m_spEodSrv->EodLogLevel;
		m_nLogHistory = m_spEodSrv->EodLogHistory;
		
		ITaskPtr spEodTask;
		m_spEodSrv->GetTask(&spEodTask);
		if(spEodTask != NULL)
		{
			m_bEodRunPeriodically = true;
			m_dtEodStartTime = spEodTask->StartTime;
		}
		else
			m_bEodRunPeriodically = false;

		m_nEodState = m_spEodSrv->EodStatus;

		__CHECK_HRESULT(DispEventAdvise(m_spEodSrv),
						_T("Fail to connect to End of Day Server events."));
	}
	catch(_com_error& e)
	{
		m_nEodState = enEodStateUnavailable;
		m_spEodSrv = NULL;

		CString strError;
		strError.Format(_T("Fail to initialize End of Day server: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//if(!CFrameWnd::PreCreateWindow(cs))
	//	return FALSE;
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

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
	return TRUE;
}


// CMainFrame message handlers
void CMainFrame::OnOptionsSettings()
{
	if(enEodStateUnavailable == m_nEodState || m_spEodSrv == NULL)
	{
		MessageBox(_T("Fail to setup settings. End of Day server is not properly initialized."), NULL, MB_ICONSTOP);
		return;
	}

	CEodSettingsDlg dlg;

	dlg.SetSteps(m_nEodSteps);
	dlg.m_nRunPeriodically = m_bEodRunPeriodically ? 1 : 0;
	dlg.m_dtStartTime = m_dtEodStartTime;
	dlg.CalcModel = m_enCalcModel;
	dlg.LogLevel = m_nLogLevel;
	dlg.LogHistory = m_nLogHistory;

	if(dlg.DoModal() == IDOK)
	{
		long nSteps = dlg.GetSteps();
		bool bEodRunPeriodically = (dlg.m_nRunPeriodically != 0);
		COleDateTime dtEodStartTime(0, 0, 0, dlg.m_dtStartTime.GetHour(),
									dlg.m_dtStartTime.GetMinute(), dlg.m_dtStartTime.GetSecond());

		long nSpanSeconds = (long)fabs(COleDateTimeSpan(m_dtEodStartTime - dtEodStartTime).GetTotalSeconds());
		EodCalcModelTypeEnum enCalcModel = dlg.CalcModel;
		int nLogLevel = dlg.LogLevel;
		int nLogHistory = dlg.LogHistory;

		if(m_nEodSteps != nSteps || m_bEodRunPeriodically != bEodRunPeriodically
			|| nSpanSeconds != 0 || m_enCalcModel != enCalcModel ||
			m_nLogLevel != nLogLevel || m_nLogHistory != nLogHistory)
		{
			try
			{
				CWaitCursor wait;

				if(m_bEodRunPeriodically != bEodRunPeriodically || nSpanSeconds != 0)
				{
					if(bEodRunPeriodically)
					{
						ITaskPtr spEodTask;
						__CHECK_HRESULT(spEodTask.CreateInstance(CLSID_Task),
										_T("Fail to create End of Day Server task object."));
						spEodTask->StartTime = dtEodStartTime;
						m_spEodSrv->AddTask(spEodTask);

						if(m_bEodRunPeriodically != bEodRunPeriodically)
						{
							CRegKey key;
							long nResult = key.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),KEY_WRITE);

							if(ERROR_SUCCESS == nResult)
							{
								TCHAR szAppPath[4096] = {0};

								if(::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, 4096))
								{
									CString strVal = szAppPath;
									strVal += _T(" /background");
									nResult = key.SetStringValue(_T("ETS End of Day Manager"), strVal);

									MessageBox(_T("End of Day Manager will be running at logon automatically."),
										NULL, MB_ICONEXCLAMATION);
								}
							}
						}
					}
					else
					{
						if(m_bEodRunPeriodically != bEodRunPeriodically)
						{
							CRegKey key;
							long nResult = key.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),KEY_WRITE);

							if(ERROR_SUCCESS == nResult)
							{
								nResult = key.DeleteValue(_T("ETS End of Day Manager"));
							}
						}

						m_spEodSrv->RemoveTask();
					}

					m_bEodRunPeriodically = bEodRunPeriodically;
					m_dtEodStartTime = dtEodStartTime;
				}

				if(m_nEodSteps != nSteps || m_enCalcModel != enCalcModel ||
					m_nLogLevel != nLogLevel || m_nLogHistory != nLogHistory)
				{
					m_spEodSrv->EodMask = nSteps;
					m_nEodSteps = nSteps;

					m_spEodSrv->EodCalcModel = enCalcModel;
					m_spEodSrv->EodLogLevel = nLogLevel;
					m_spEodSrv->EodLogHistory = nLogHistory;
					m_enCalcModel = enCalcModel;
					m_nLogLevel = nLogLevel;
					m_nLogHistory = nLogHistory;

					MessageBox(_T("New settings will be actual on next start of End of Day server."),
								NULL, MB_ICONEXCLAMATION);
				}
			}
			catch(_com_error& e)
			{
				CString strError;
				strError.Format(_T("Fail to apply End of Day server settings: %s (%X)"),
					(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
				MessageBox(strError, NULL, MB_ICONSTOP);
			}
		}
	}
}

void CMainFrame::OnOptionsConnections()
{
	CConnectionsDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bIsDbChanged || dlg.m_bIsPpChanged || dlg.m_bIsIvChanged || dlg.m_bIsVsChanged)
			MessageBox(_T("New settings will be actual on next start of End of Day server."),
						NULL, MB_ICONEXCLAMATION);
	}
}

void CMainFrame::OnDestroy()
{
	try
	{
		if(m_spEodSrv != NULL)
		{
			DispEventUnadvise(m_spEodSrv);
			m_spEodSrv = NULL;
		}
	}
	catch (_com_error& e)
	{
		CString strError;
		strError.Format(_T("Fail to uninitialize Eod Server: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}

    g_App.PersistWindowPlacement(this, _T("MainWindow"), true);
	CFrameWnd::OnDestroy();
}

void CMainFrame::OnFirstActivate()
{
	CEtsEodManagerView* pView = reinterpret_cast<CEtsEodManagerView*>(GetActiveView());
	if(pView)
		pView->AdjustLastColWidth();
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

void CMainFrame::OnFileStartEod()
{
	if(enEodStateUnavailable == m_nEodState || m_spEodSrv == NULL)
		MessageBox(_T("End of Day server is not properly initialized."), NULL, MB_ICONSTOP);
	else if(enEodStateStartPending == m_nEodState || enEodStateStopPending == m_nEodState)
		MessageBox(_T("End of Day server is busy now."), NULL, MB_ICONEXCLAMATION);
	else if(enEodStateStopped == m_nEodState)
	{
		try
		{
			CWaitCursor wait;

			CEtsEodManagerView* pView = reinterpret_cast<CEtsEodManagerView*>(GetActiveView());
			if(pView)
			{
				CEtsEodManagerDoc* pDoc = pView->GetDocument();
				if(pDoc)
					pDoc->EnableNotify(FALSE);
			}

			__CHECK_HRESULT(m_spEodSrv->StartEod(), _T(""));

			//PostMessage(WM_COMMAND, ID_FILE_NEW, 0);

			m_nEodState = m_spEodSrv->EodStatus;
		}
		catch(_com_error& e)
		{
			m_nEodState = enEodStateStopped;
			CString strError;
			strError.Format(_T("Fail to start End of Day server: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CMainFrame::OnUpdateFileStartEod(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->Enable(enEodStateStopped == m_nEodState);
}

void CMainFrame::OnFileStopEod()
{
	if(enEodStateUnavailable == m_nEodState || m_spEodSrv == NULL)
		MessageBox(_T("End of Day server is not properly initialized."), NULL, MB_ICONSTOP);
	else if(enEodStateStartPending == m_nEodState || enEodStateStopPending == m_nEodState)
		MessageBox(_T("End of Day server is busy now."), NULL, MB_ICONEXCLAMATION);
	else if(enEodStateStarted == m_nEodState)
	{
		try
		{
			CWaitCursor wait;
			__CHECK_HRESULT(m_spEodSrv->StopEod(), _T(""));
			m_nEodState = m_spEodSrv->EodStatus;
		}
		catch(_com_error& e)
		{
			m_nEodState = enEodStateStarted;
			CString strError;
			strError.Format(_T("Fail to stop End of Day server: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
}

void CMainFrame::OnUpdateFileStopEod(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->Enable(enEodStateStarted == m_nEodState);
}

STDMETHODIMP CMainFrame::OnEodStatusChanged(EodStatusEnum enStatus)
{
	m_nEodState = enStatus;
	
	if(enEodStateStartPending == m_nEodState)
			PostMessage(WM_COMMAND, ID_FILE_NEW, 0);

	return S_OK;
}

STDMETHODIMP CMainFrame::OnEodError(BSTR bsDescription)
{
	_bstr_t bstrError(bsDescription);
	CString strError;
	strError.Format(_T("End of Day error: %s."), (LPCTSTR)bstrError);
	MessageBox(strError, NULL, MB_ICONSTOP);
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

bool CMainFrame::IsTimerSet()
{
	if(m_spEodSrv)
	{
		try
		{
			ITaskPtr spEodTask;
			m_spEodSrv->GetTask(&spEodTask);
			if(spEodTask != NULL)
				return true;
		}
		catch (_com_error& e) 
		{
			CString strError;
			strError.Format(_T("Fail to check status of End of Day timer: %s (%X)"),
				(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
			MessageBox(strError, NULL, MB_ICONSTOP);
		}
	}
	return false;
}