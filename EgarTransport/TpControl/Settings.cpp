// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettings

IMPLEMENT_DYNAMIC(CSettings, CEgPropSheet)

CSettings::CSettings(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CEgPropSheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_IsOK = false;
	m_IsChanged = false;
	AddPages();
}

CSettings::CSettings(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CEgPropSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_IsOK = false;
	m_IsChanged = false;
	AddPages();
}

CSettings::~CSettings()
{
}

void CSettings::AddPages()
{
	AddPage(&m_AboutPage);
	AddPage(&m_HostStPage);
	AddPage( &m_ServicePage );
	AddPage( &m_HostPage );
	AddPage( &m_RouterPage );
	AddPage( &m_TracePage);
}


BEGIN_MESSAGE_MAP(CSettings, CEgPropSheet)
	//{{AFX_MSG_MAP(CSettings)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_REFRESH_PROPSHEET, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers
void CSettings::OnApplyNow()
{
	if(!GetActivePage()->UpdateData())
		return;

	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd)
		pWnd->SendMessage(WM_HOST_SETTINGS_UPDATE, 0, 0);
	if(IsWindow(m_HostPage.m_hWnd))
		m_HostPage.SetModified(FALSE);
	if(IsWindow(m_RouterPage.m_hWnd))
		m_RouterPage.SetModified(FALSE);
	if(IsWindow(m_TracePage.m_hWnd))
		m_TracePage.SetModified(FALSE);

	
	m_IsChanged = false;
	if(IsWindow(m_hWnd))
		DisableCancel();
}

void CSettings::OnClose() 
{
	ShowWindow(SW_HIDE);
}

void CSettings::OnCancel()
{
	if(IsWindow(m_RouterPage.m_hWnd))
		m_RouterPage.ResetSelection();
	ShowWindow(SW_HIDE);
}

void CSettings::OnOK()
{
	if(!m_IsOK || !m_IsChanged )
	{
		ShowWindow(SW_HIDE);
		return;
	}

	if(!GetActivePage()->UpdateData())
		return;
	
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd)
		pWnd->SendMessage(WM_HOST_SETTINGS_UPDATE, 0, 0);

	m_HostPage.SetModified(FALSE);
	m_RouterPage.SetModified(FALSE);
	m_TracePage.SetModified(FALSE);

	ShowWindow(SW_HIDE);
}

LRESULT CSettings::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	m_IsChanged = false;
	
	EnableCancel();
	
	SetActivePage(0);

	ShowWindow(SW_SHOW);

	return 0;
}

void CSettings::DisableCancel()
{
	if(!IsWindow(m_hWnd))
		return;

	CWnd* pButton = GetDlgItem (IDCANCEL);
	if(pButton)
		pButton->EnableWindow(FALSE);

	pButton = GetDlgItem (IDOK);
	if(pButton)
	{
		pButton->SetWindowText(_T("Close"));
		pButton->EnableWindow(TRUE);
	}
	m_IsOK = false;
}

void CSettings::EnableCancel()
{
	CWnd* pButton = GetDlgItem (IDCANCEL);
	if(pButton)
		pButton->EnableWindow(TRUE);

	pButton = GetDlgItem (ID_APPLY_NOW);
	if(pButton)
		pButton->EnableWindow(FALSE);
	pButton = GetDlgItem (IDOK);
	if(pButton)
	{
		pButton->SetWindowText(_T("OK"));
		pButton->EnableWindow(TRUE);
	}
	m_IsOK = true;
}
