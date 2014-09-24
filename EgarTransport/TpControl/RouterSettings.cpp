// RouterSettings.cpp : implementation file
//

#include "stdafx.h"
#include "RouterSettings.h"
#include "Settings.h"
#include "TransportControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRouterSettings property page

IMPLEMENT_DYNCREATE(CRouterSettings, CPropertyPage)

CRouterSettings::CRouterSettings() : CPropertyPage(CRouterSettings::IDD)
{
	//{{AFX_DATA_INIT(CRouterSettings)
	m_dwCheckHostTime = 0;
	m_dwConnTimeout = 0;
	m_dwLocRouterPort = 0;
	//}}AFX_DATA_INIT
}

CRouterSettings::~CRouterSettings()
{
}

void CRouterSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRouterSettings)
	DDX_Control(pDX, IDC_LOCROUTERADDR, m_cmbLocRouterAddr);
	DDX_Text(pDX, IDC_CHECKHOSTM, m_dwCheckHostTime);
	DDV_MinMaxDWord(pDX, m_dwCheckHostTime, 1, 3600);
	DDX_Text(pDX, IDC_CONNTIMEOUT, m_dwConnTimeout);
	DDV_MinMaxDWord(pDX, m_dwConnTimeout, 1, 86400);
	DDX_Text(pDX, IDC_LOCROUTERPORT, m_dwLocRouterPort);
	DDV_MinMaxDWord(pDX, m_dwLocRouterPort, 1, 64000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRouterSettings, CPropertyPage)
	//{{AFX_MSG_MAP(CRouterSettings)
	ON_EN_CHANGE(IDC_CHECKHOSTM, OnChange)
	ON_EN_CHANGE(IDC_CONNTIMEOUT, OnChange)
	ON_EN_CHANGE(IDC_LOCROUTERPORT, OnChange)
	ON_CBN_SELCHANGE(IDC_LOCROUTERADDR, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRouterSettings message handlers

void CRouterSettings::OnChange() 
{
	SetModified();
	CSettings* pParent = (CSettings*)GetParent();
	if(pParent)
	{
		pParent->m_IsChanged = true;
	}
}

BOOL CRouterSettings::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CTpControlApp* pApp = (CTpControlApp*)AfxGetApp();
	if(pApp)
	{
		CString strAddr;
		DWORD dwAddr;
		if(pApp->GetFirstIPAddress(dwAddr,strAddr))
		{
			do
			{
				int index = m_cmbLocRouterAddr.AddString(strAddr);
				if(index != CB_ERR)
				{
					m_mapIPIndex[dwAddr] = index;
					m_cmbLocRouterAddr.SetItemData(index,dwAddr);
				}

			}while(pApp->GetNextIPAddress(dwAddr,strAddr));
		}
		m_cmbLocRouterAddr.SetCurSel(m_mapIPIndex[m_dwAddress]);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CRouterSettings::ResetSelection()
{
	m_cmbLocRouterAddr.SetCurSel(m_mapIPIndex[m_dwAddress]);
}
