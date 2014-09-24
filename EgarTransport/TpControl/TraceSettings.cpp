// TraceSettings.cpp : implementation file
//

#include "stdafx.h"
#include "TraceSettings.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraceSettings property page

IMPLEMENT_DYNCREATE(CTraceSettings, CPropertyPage)

CTraceSettings::CTraceSettings() : CPropertyPage(CTraceSettings::IDD)
{
	//{{AFX_DATA_INIT(CTraceSettings)
	m_strLogDir = _T("");
	m_lLogHistory = 0;
	m_lLogLevel = 0;
	//}}AFX_DATA_INIT
}

CTraceSettings::~CTraceSettings()
{
}

void CTraceSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraceSettings)
	DDX_Text(pDX, IDC_LOGHISTORY, m_lLogHistory);
	DDV_MinMaxLong(pDX, m_lLogHistory, 1, 10000);
	DDX_Text(pDX, IDC_LOGLEVEL, m_lLogLevel);
	DDV_MinMaxLong(pDX, m_lLogLevel, 0, 10);
	DDX_Text(pDX, IDC_LOGDIR, m_strLogDir);
	DDV_MaxChars(pDX, m_strLogDir, 256);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate && m_strLogDir.GetLength() < 2)
	{
		CString prompt = _T("Too short log files directory path.");
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		pDX->Fail();
	}
}


BEGIN_MESSAGE_MAP(CTraceSettings, CPropertyPage)
	//{{AFX_MSG_MAP(CTraceSettings)
	ON_EN_CHANGE(IDC_LOGDIR, OnChange)
	ON_EN_CHANGE(IDC_LOGHISTORY, OnChange)
	ON_EN_CHANGE(IDC_LOGLEVEL, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceSettings message handlers

void CTraceSettings::OnChange() 
{
	SetModified();
	CSettings* pParent = (CSettings*)GetParent();
	if(pParent)
	{
		pParent->m_IsChanged = true;
	}
}
