// HostSettings.cpp : implementation file
//

#include "stdafx.h"
#include "HostSettings.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostSettings property page

IMPLEMENT_DYNCREATE(CHostSettings, CPropertyPage)

CHostSettings::CHostSettings() : CPropertyPage(CHostSettings::IDD)
{
	//{{AFX_DATA_INIT(CHostSettings)
	m_dwConnLoseTm = 0;
	m_dwConnTimeout = 0;
	m_dwTestRequestTm = 0;
	m_dwReconnTm = 0;
	m_strRouterHost = _T("");
	m_dwRouterPort = 0;
	//}}AFX_DATA_INIT
}

CHostSettings::~CHostSettings()
{
}

void CHostSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHostSettings)
	DDX_Text(pDX, IDC_CONNLOSETM, m_dwConnLoseTm);
	DDV_MinMaxDWord(pDX, m_dwConnLoseTm, 1, 3600);
	DDX_Text(pDX, IDC_CONNTIMEOUT, m_dwConnTimeout);
	DDV_MinMaxDWord(pDX, m_dwConnTimeout, 1, 3600);
	DDX_Text(pDX, IDC_TESTREQUESTTM, m_dwTestRequestTm);
	DDV_MinMaxDWord(pDX, m_dwTestRequestTm, 1, 3600);
	DDX_Text(pDX, IDC_RECONNTM, m_dwReconnTm);
	DDV_MinMaxDWord(pDX, m_dwReconnTm, 1, 3600);
	DDX_Text(pDX, IDC_ROUTERHOST, m_strRouterHost);
	DDV_MaxChars(pDX, m_strRouterHost, 100);
	DDX_Text(pDX, IDC_ROUTERPORT, m_dwRouterPort);
	DDV_MinMaxDWord(pDX, m_dwRouterPort, 1, 64000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHostSettings, CPropertyPage)
	//{{AFX_MSG_MAP(CHostSettings)
	ON_EN_CHANGE(IDC_CONNLOSETM, OnChange)
	ON_EN_CHANGE(IDC_CONNTIMEOUT, OnChange)
	ON_EN_CHANGE(IDC_TESTREQUESTTM, OnChange)
	ON_EN_CHANGE(IDC_RECONNTM, OnChange)
	ON_EN_CHANGE(IDC_ROUTERHOST, OnChange)
	ON_EN_CHANGE(IDC_ROUTERPORT, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHostSettings message handlers

void CHostSettings::OnChange() 
{
	SetModified();
	CSettings* pParent = (CSettings*)GetParent();
	if(pParent)
	{
		pParent->m_IsChanged = true;
	}
}
