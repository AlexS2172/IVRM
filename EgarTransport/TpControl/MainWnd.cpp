#include "stdafx.h"
#include "TransportControl.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CImageList	g_ilConnectStatus;

IMPLEMENT_DYNCREATE(CMainWnd, CFrameWnd)

CMainWnd::CMainWnd() : m_SettingsSheet(_T("Transport control")), 
m_IsMessBoxActive(false), m_uIDTimer(0)
{
}

CMainWnd::~CMainWnd()
{
}

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_COMMAND(RT_STATUS_ONLINE, OnRtOnline)
	ON_COMMAND(RT_STATUS_OFFLINE, OnRtOffline)
	ON_COMMAND(TRANSPORT_DISABLED, OnTransportDisabled)
	ON_MESSAGE(WM_HOST_SETTINGS_UPDATE, OnSettingsUpdate)
	ON_MESSAGE(WM_HOST_STATISTIC_UPDATE, OnUpdateStatistic)
	ON_COMMAND(ID_SETTINGS, OnTransportSettings)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TRANSPORT_EXIT, OnExit)
	ON_MESSAGE(WM_TRANSPORT_SERVICE_ERROR, OnTransportServiceError)
	ON_MESSAGE(WM_TRANSPORT_SERVICE_STATUS, OnTransportServiceStatus)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the tray icon
	if (!m_TrayIcon.Create(NULL, WM_ICON_NOTIFY, _T("Transport control"), NULL, IDR_TRAY_POPUP_MENU))			
		return -1;

	// Set tray icon to status 'disabled' 
	m_TrayIcon.SetIcon(IDI_DISABLED_START);

	if(!m_SettingsSheet.Create(this,WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME))
		return -1;

	CString format,text;
	
	if(theApp)
	{
		CString strRouterName;

		if(SUCCEEDED(theApp.GetHostStatus(strRouterName)))
		{

			switch (theApp.getTpStatus())
			{
				case enHSOnline:
				{
					m_TrayIcon.SetIcon(IDI_ONLINE);
		
					format.LoadString(IDS_ONLINE);
					text.Format(format,strRouterName);

					SetWindowText(text);
				}
				break;
				case enHSOffline:
				{
					if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
					{

						m_TrayIcon.SetIcon(IDI_OFFLINE_START);
						format.LoadString(IDS_OFFLINE_START);
						text.Format(format,strRouterName);
					}
					else
					{
						m_TrayIcon.SetIcon(IDI_OFFLINE_STOP);
						text.LoadString(IDS_OFFLINE_STOP);
					}
					SetWindowText(text);

				}
				break;
				case enHSDisabled:
				{
					if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
					{
						m_TrayIcon.SetIcon(IDI_DISABLED_START);
						text.LoadString(IDS_DISABLED_START);
					}
					else
					{
						m_TrayIcon.SetIcon(IDI_DISABLED_STOP);
						text.LoadString(IDS_DISABLED_STOP);
					}
					SetWindowText(text);
				}
				break;
			}
		}
		else
		{
			m_TrayIcon.SetIcon(IDI_DISABLED_STOP);
			text.LoadString(IDS_DISABLED_STOP);
			SetWindowText(text);
		}
	}
	StartTimer();

	return 0;
}

void CMainWnd::OnClose() 
{	
	StopTimer();
	m_SettingsSheet.DestroyWindow();
	CFrameWnd::OnClose();
}

void CMainWnd::OnRtOnline()
{
	m_TrayIcon.SetIcon(IDI_ONLINE);
	return;
}

void CMainWnd::OnRtOffline()
{
	if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
		m_TrayIcon.SetIcon(IDI_OFFLINE_START);
	else
		m_TrayIcon.SetIcon(IDI_OFFLINE_STOP);

	return;
}

void CMainWnd::OnTransportDisabled()
{
	if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
		m_TrayIcon.SetIcon(IDI_DISABLED_START);
	else
		m_TrayIcon.SetIcon(IDI_DISABLED_STOP);
	
	if(IsWindow(m_SettingsSheet.m_hWnd))
	{
		m_SettingsSheet.m_HostStPage.Update();
	}
	
	return;
}

LRESULT CMainWnd::OnSettingsUpdate(WPARAM wParam, LPARAM lParam)
{
	try
	{
		if(IsWindow(m_SettingsSheet))
		{
			CString strMess,strCaption;
			strMess.LoadString(IDS_SETTINGS_NOTE);
			strCaption.LoadString(IDS_PROJNAME);
			
			m_IsMessBoxActive = true;

			int Ret = IDYES;

			if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
				Ret = MessageBox(strMess, strCaption,MB_ICONWARNING | MB_SETFOREGROUND | MB_YESNO);

			m_IsMessBoxActive = false;
			
			//Host settings

			CTpControlApp::SetSettingsToReg(SETTINGS_CONNECTION_LOSE_TIMEOUT,m_SettingsSheet.m_HostPage.m_dwConnLoseTm);
			CTpControlApp::SetSettingsToReg(SETTINGS_CONNECT_TIMEOUT,m_SettingsSheet.m_HostPage.m_dwConnTimeout);
			CTpControlApp::SetSettingsToReg(SETTINGS_RECONNECT_TIME,m_SettingsSheet.m_HostPage.m_dwReconnTm);
			CTpControlApp::SetSettingsToReg(SETTINGS_TESTREQUEST_TIMEOUT,m_SettingsSheet.m_HostPage.m_dwTestRequestTm);
			CTpControlApp::SetSettingsToReg(SETTINGS_NETWORK_ROUTER_PORT,m_SettingsSheet.m_HostPage.m_dwRouterPort);
			CTpControlApp::SetRouterHostName(_bstr_t(m_SettingsSheet.m_HostPage.m_strRouterHost));
			//Router settings
			CTpControlApp::SetSettingsToReg(SETTINGS_CHECK_HOSTS_CONN_PERIOD,m_SettingsSheet.m_RouterPage.m_dwCheckHostTime);
			CTpControlApp::SetSettingsToReg(SETTINGS_HOST_CONN_EXP_TIMEOUT,m_SettingsSheet.m_RouterPage.m_dwConnTimeout);
			CTpControlApp::SetSettingsToReg(SETTINGS_LOCAL_ROUTER_PORT,m_SettingsSheet.m_RouterPage.m_dwLocRouterPort);	

			if(IsWindow(m_SettingsSheet.m_RouterPage.m_cmbLocRouterAddr.m_hWnd))
			{
				CTpControlApp::SetSettingsToReg(SETTINGS_LOCAL_ROUTER_ADDR,m_SettingsSheet.m_RouterPage.m_cmbLocRouterAddr.GetItemData(
								m_SettingsSheet.m_RouterPage.m_cmbLocRouterAddr.GetCurSel()));
			}
			//Trace settings
			CTpControlApp::SetSettingsToReg(SETTINGS_LOGS_HISTORY,m_SettingsSheet.m_TracePage.m_lLogHistory,SETTINGS_LOGS_REGISTRY_KEY);
			CTpControlApp::SetSettingsToReg(SETTINGS_LOGS_MIN_LEVEL,m_SettingsSheet.m_TracePage.m_lLogLevel,SETTINGS_LOGS_REGISTRY_KEY);
			CTpControlApp::SetSettingsToReg(SETTINGS_LOGS_DIRECTORY,m_SettingsSheet.m_TracePage.m_strLogDir,SETTINGS_LOGS_REGISTRY_KEY);

			if(Ret ==  IDYES)
				theApp.RestartTransportService();
		}
	}
	catch(_com_error er)
	{
	}
	return 0;
}
void CMainWnd::OnTransportSettings()
{	
	if(m_SettingsSheet.IsWindowVisible())
	{
		if(!m_IsMessBoxActive)
			m_SettingsSheet.SetActiveWindow();
	}
 	else
	{
		//Host settings
		m_SettingsSheet.m_HostPage.m_dwConnLoseTm = CTpControlApp::GetSettingsFromReg(SETTINGS_CONNECTION_LOSE_TIMEOUT,CONNECTION_LOSE_TIMEOUT);
		m_SettingsSheet.m_HostPage.m_dwConnTimeout = CTpControlApp::GetSettingsFromReg(SETTINGS_CONNECT_TIMEOUT,CONNECT_TIMEOUT);
		m_SettingsSheet.m_HostPage.m_dwReconnTm = CTpControlApp::GetSettingsFromReg(SETTINGS_RECONNECT_TIME,RECONNECT_TIME);
		m_SettingsSheet.m_HostPage.m_dwTestRequestTm = CTpControlApp::GetSettingsFromReg(SETTINGS_TESTREQUEST_TIMEOUT,TESTREQUEST_TIMEOUT);
		m_SettingsSheet.m_HostPage.m_dwRouterPort = CTpControlApp::GetSettingsFromReg(SETTINGS_NETWORK_ROUTER_PORT,ROUTER_PORT_NUMBER);

		_bstr_t bsHostName;
		CTpControlApp::GetRouterHostName(bsHostName);
		m_SettingsSheet.m_HostPage.m_strRouterHost = (LPTSTR)bsHostName;

		//Router settings
		m_SettingsSheet.m_RouterPage.m_dwCheckHostTime = CTpControlApp::GetSettingsFromReg(SETTINGS_CHECK_HOSTS_CONN_PERIOD,CHECK_HOSTS_CONN_PERIOD);
		m_SettingsSheet.m_RouterPage.m_dwConnTimeout = CTpControlApp::GetSettingsFromReg(SETTINGS_HOST_CONN_EXP_TIMEOUT,HOST_CONN_EXP_TIMEOUT);
		m_SettingsSheet.m_RouterPage.m_dwLocRouterPort = CTpControlApp::GetSettingsFromReg(SETTINGS_LOCAL_ROUTER_PORT,ROUTER_PORT_NUMBER);
		
		m_SettingsSheet.m_RouterPage.m_dwAddress  = CTpControlApp::GetSettingsFromReg(SETTINGS_LOCAL_ROUTER_ADDR,INADDR_ANY);

		//Transport settings
		m_SettingsSheet.m_TracePage.m_lLogHistory = CTpControlApp::GetSettingsFromReg(SETTINGS_LOGS_HISTORY,7,SETTINGS_LOGS_REGISTRY_KEY);
		m_SettingsSheet.m_TracePage.m_lLogLevel = CTpControlApp::GetSettingsFromReg(SETTINGS_LOGS_MIN_LEVEL,0,SETTINGS_LOGS_REGISTRY_KEY);
		m_SettingsSheet.m_TracePage.m_strLogDir = CTpControlApp::GetSettingsFromReg(SETTINGS_LOGS_DIRECTORY,_T(""),SETTINGS_LOGS_REGISTRY_KEY);

		if(!m_SettingsSheet.m_TracePage.m_strLogDir.GetLength())
		{
			TCHAR szBuf[_MAX_PATH] = {0};
			
			DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
			if(dwErr)
			{
				TCHAR szDrive[_MAX_DRIVE];
				TCHAR szDir[_MAX_DIR];
				TCHAR szFname[_MAX_FNAME];
				TCHAR szExt[_MAX_EXT];
				_tsplitpath_s( szBuf, szDrive,sizeof(szDrive), szDir,sizeof(szDir), szFname,sizeof(szFname), szExt, sizeof(szExt) );

				m_SettingsSheet.m_TracePage.m_strLogDir.Format(_T("%s%sLogs"),szDrive, szDir);
			}
		}

		if(IsWindow(m_SettingsSheet.m_HostPage.m_hWnd))
			m_SettingsSheet.m_HostPage.UpdateData(FALSE);
	
		if(IsWindow(m_SettingsSheet.m_RouterPage.m_hWnd))
			m_SettingsSheet.m_RouterPage.UpdateData(FALSE);

		if(IsWindow(m_SettingsSheet.m_TracePage.m_hWnd))
			m_SettingsSheet.m_TracePage.UpdateData(FALSE);		

		m_SettingsSheet.SendMessage(WM_REFRESH_PROPSHEET, 0, 0);
	}
}


LRESULT CMainWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if (message == WM_SETTEXT)
    {
		m_TrayIcon.SetTooltipText((LPCTSTR)lParam);
		
		if(IsWindow(m_SettingsSheet.m_hWnd))
		{
			m_SettingsSheet.SetWindowText((LPCTSTR)lParam);
		}
    }
 	
	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}

void CMainWnd::OnExit() 
{
	if(!m_IsMessBoxActive)
		SendMessage(WM_CLOSE);	
}
	
LRESULT CMainWnd::OnUpdateStatistic(WPARAM wParam, LPARAM lParam)
{
	if(IsWindow(m_SettingsSheet.m_hWnd))
	{
		m_SettingsSheet.m_HostStPage.Update();
	}
	return 0;
}

LRESULT CMainWnd::OnTransportServiceError(WPARAM wParam, LPARAM lParam)
{
	CString strError,strCaption;
			
	strCaption.LoadString(IDS_PROJNAME);

	strError = (LPCTSTR)lParam;

	MessageBox(strError,strCaption,MB_OK | MB_ICONSTOP);
			
	return 0;
}

LRESULT CMainWnd::OnTransportServiceStatus(WPARAM wParam, LPARAM lParam)
{
	m_SettingsSheet.m_ServicePage.UpdateTpStatus();

	CString strRouterName;

	theApp.GetHostStatus(strRouterName);

	CString text,format;

	if(theApp.getTpSrvStatus() == SERVICE_RUNNING)
	{
		switch (theApp.getTpStatus())
		{
			case enHSOnline:
			{
				m_TrayIcon.SetIcon(IDI_ONLINE);
	
				format.LoadString(IDS_ONLINE);
				text.Format(format,strRouterName);
			}
			break;
			case enHSOffline:
			{
				m_TrayIcon.SetIcon(IDI_OFFLINE_START);
				format.LoadString(IDS_OFFLINE_START);
				text.Format(format,strRouterName);
			}
			break;
			case enHSDisabled:
			{
				m_TrayIcon.SetIcon(IDI_DISABLED_START);
				text.LoadString(IDS_DISABLED_START);
			}
			break;
		}
	}
	else
	{
		if(enHSDisabled == theApp.getTpStatus())
		{
			m_TrayIcon.SetIcon(IDI_DISABLED_STOP);
			text.LoadString(IDS_DISABLED_STOP);
		}
		else
		{
			m_TrayIcon.SetIcon(IDI_OFFLINE_STOP);
			text.LoadString(IDS_OFFLINE_STOP);
		}
	}
	SetWindowText(text);
	
	return 0;
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	StopTimer();
	try
	{
		CTpControlApp* pApp = (CTpControlApp*)::AfxGetApp();
		if(pApp)
		{
			pApp->GetStatistic();

			if(IsWindow(m_SettingsSheet.m_hWnd))
			{
				m_SettingsSheet.m_HostStPage.Update();
			}

			CString strRouterName;

			if(SUCCEEDED(pApp->GetHostStatus(strRouterName)))
			{
				if (enHSOnline == pApp->getTpStatus())
				{
					PostMessage(WM_COMMAND, RT_STATUS_ONLINE);

					CString format,text;
					format.LoadString(IDS_ONLINE);
					text.Format(format,strRouterName);

					SetWindowText(text);
				}
			}

		}

		CFrameWnd::OnTimer(nIDEvent);
	}
	catch(...){}
	StartTimer();
}
