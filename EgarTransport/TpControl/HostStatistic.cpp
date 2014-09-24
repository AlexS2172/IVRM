// HostStatistic.cpp : implementation file
//

#include "stdafx.h"
#include "TransportControl.h"
#include "HostStatistic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostStatistic dialog
IMPLEMENT_DYNCREATE(CHostStatistic, CPropertyPage)


CHostStatistic::CHostStatistic(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CHostStatistic::IDD)
{
	//{{AFX_DATA_INIT(CHostStatistic)
	m_sStartLogonTime = _T("");
	m_sConfirmedMsg = _T("");
	m_sReceivedMsg = _T("");
	m_sUnsentMsg = _T("");
	m_sSentMsg = _T("");
	//}}AFX_DATA_INIT
}


void CHostStatistic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHostStatistic)
	DDX_Text(pDX, IDC_START_LOGON_TIME, m_sStartLogonTime);
	DDX_Text(pDX, IDC_CONF_MSG, m_sConfirmedMsg);
	DDX_Text(pDX, IDC_RECEIVE_MSG, m_sReceivedMsg);
	DDX_Text(pDX, IDC_UNSENT_MSG, m_sUnsentMsg);
	DDX_Text(pDX, IDC_SENT_MSG, m_sSentMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHostStatistic, CPropertyPage)
	//{{AFX_MSG_MAP(CHostStatistic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHostStatistic::Update()
{	
	CAutoLock lock(m_Lock);
	
	m_sStartLogonTime = "";

	if (theApp.m_dtLogonTime != 0)
	{	
		// convert to local time

		COleDateTime date = theApp.m_dtLogonTime;
		
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

	unsigned long nSMN = theApp.m_ulSendMessNum;

	m_sSentMsg.Format(_T("%d"), nSMN);

	unsigned long nCMN = theApp.m_ulConfMessNum;

	m_sConfirmedMsg.Format(_T("%d"), nCMN);

	unsigned long nRMN = theApp.m_ulRecMessNum;

	m_sReceivedMsg.Format(_T("%d"),nRMN);

	unsigned long nUMN = theApp.m_ulUnsentMessNum;
	m_sUnsentMsg.Format(_T("%d"),nUMN);
	
	if(IsWindow(m_hWnd))
		UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CHostStatistic message handlers

BOOL CHostStatistic::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
