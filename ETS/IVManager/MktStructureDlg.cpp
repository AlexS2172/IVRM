// MktStructureDlg.cpp : implementation file
//
 
#include "stdafx.h"
#include "ivanalyzer.h"
#include "MktStructureDlg.h"
#include "DBLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_START_DOWNLOAD WM_USER+1

/////////////////////////////////////////////////////////////////////////////
// CMktStructureDlg dialog


CMktStructureDlg::CMktStructureDlg(LPCTSTR szSymbol /*=NULL*/, int nID /*=0*/, CWnd* pParent /*=NULL*/)
	: CMktStructureBaseDlg(CMktStructureDlg::IDD, pParent) 
{
	if (szSymbol)
		m_stocks [szSymbol] = nID;
	
	//{{AFX_DATA_INIT(CMktStructureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

bool CMktStructureDlg::IsSymbolUpdated (LPCTSTR szSymbol)
{
	bool bResult = false;
	stocks_map::iterator it = m_stocks.find (szSymbol);
	bResult = ( it == m_stocks.end() );
	return bResult;
}

void CMktStructureDlg::DoDataExchange(CDataExchange* pDX)
{
	CMktStructureBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMktStructureDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMktStructureDlg, CMktStructureBaseDlg)
	//{{AFX_MSG_MAP(CMktStructureDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_START_DOWNLOAD, OnStartDownload)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMktStructureDlg message handlers

BOOL CMktStructureDlg::OnInitDialog() 
{
	if (m_stocks.size() == 1 )
	{
		CString strCaption;
		AfxFormatString1 (strCaption, IDS_UPDATING_ACTIVE_SYMBOL, (LPCTSTR)  m_stocks.begin()->first);
		SetWindowText ( strCaption);
	}

	CMktStructureBaseDlg::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CMktStructureDlg::OnStartDownload (WPARAM wParam, LPARAM lParam)
{
	StartDownload();
	return 0;
}

BOOL CMktStructureDlg::ContinueModal()
{
	if (!m_bStarted)
	{
		if (IsWindowVisible())
		{
			PostMessage (WM_START_DOWNLOAD);
			m_bStarted = true;
		}
	}

	return CMktStructureBaseDlg::ContinueModal();
}




