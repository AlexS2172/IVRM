// HostStatistics.cpp : implementation file
//

#include "stdafx.h"
#include "rtcontrol.h"
#include "HostStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostStatistics dialog


CHostStatistics::CHostStatistics(BSTR bsHostIP, CWnd* pParent /*=NULL*/)
	: CDialog(CHostStatistics::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHostStatistics)
	m_sConfirmedMsg = _T("");
	m_sReceivedMsg = _T("");
	m_sSentMsg = _T("");
	m_sStartLogonTime = _T("");
	m_sUnsentMsg = _T("");
	m_sHostIP = bsHostIP;
	//}}AFX_DATA_INIT
}


void CHostStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHostStatistics)
	DDX_Text(pDX, IDC_CONF_MSG, m_sConfirmedMsg);
	DDX_Text(pDX, IDC_RECEIVE_MSG, m_sReceivedMsg);
	DDX_Text(pDX, IDC_SENT_MSG, m_sSentMsg);
	DDX_Text(pDX, IDC_START_LOGON_TIME, m_sStartLogonTime);
	DDX_Text(pDX, IDC_UNSENT_MSG, m_sUnsentMsg);
	DDX_Text(pDX, IDC_HOST_IP, m_sHostIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHostStatistics, CDialog)
	//{{AFX_MSG_MAP(CHostStatistics)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHostStatistics message handlers


void CHostStatistics::Update()
{		
	m_sStartLogonTime = "";

	if (g_App.m_dtLogonTime != 0)
	{	
		// convert to local time

		COleDateTime date = g_App.m_dtLogonTime;
		
		SYSTEMTIME sysTime;
		FILETIME fileTime;
		if(date.GetAsSystemTime(sysTime))
		{
			if(SystemTimeToFileTime(&sysTime,&fileTime))
			{
				if(FileTimeToLocalFileTime(&fileTime,&fileTime))
				{
					if(FileTimeToSystemTime(&fileTime,&sysTime))
					{
						date = sysTime;
						m_sStartLogonTime = date.Format(_T("%m/%d/%Y %H:%M:%S"));
					}
				}
			}
		}
	}

	unsigned long nSMN = g_App.m_lSendMessNum;

	m_sSentMsg.Format(_T("%d"), nSMN);

	unsigned long nCMN = g_App.m_lConfMessNum;

	m_sConfirmedMsg.Format(_T("%d"), nCMN);

	unsigned long nRMN = g_App.m_lRecMessNum;

	m_sReceivedMsg.Format(_T("%d"),nRMN);

	unsigned long nUMN = g_App.m_lUnsentMessNum;
	m_sUnsentMsg.Format(_T("%d"),nUMN);
	
	if(IsWindow(m_hWnd))
		UpdateData(FALSE);
}

void CHostStatistics::OnTimer(UINT nIDEvent) 
{
	StopTimer();
	try
	{
		g_App.GetHostStatistic(_bstr_t(m_sHostIP));
	}
	catch(...)
	{
	}
	Update();
	CDialog::OnTimer(nIDEvent);
	StartTimer();
}

void CHostStatistics::OnClose() 
{
	StopTimer();

	CDialog::OnClose();
}

BOOL CHostStatistics::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	StartTimer();
	
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
