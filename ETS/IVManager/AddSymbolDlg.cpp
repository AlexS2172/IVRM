// AddSymbolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "AddSymbolDlg.h"
#include "DBLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSymbolDlg dialog

CAddSymbolDlg::CAddSymbolDlg(LPCTSTR szSymbol, CWnd* pParent /*=NULL*/)
	: CMktStructureBaseDlg(CAddSymbolDlg::IDD, pParent),
	m_bAdded (false), m_bRequestingData (false), m_bAddedSomething (false)
{
	//{{AFX_DATA_INIT(CAddSymbolDlg)
	//}}AFX_DATA_INIT
	if(szSymbol)
	{
		m_strNewSymbol = szSymbol;
	}
}


void CAddSymbolDlg::DoDataExchange(CDataExchange* pDX)
{
	CMktStructureBaseDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAddSymbolDlg)
	DDX_Control(pDX, IDC_ADD_SYMBOL, m_btAdd);
	DDX_Control(pDX, IDC_NEW_SYMBOL, m_edNewSymbol);
	DDX_Text(pDX, IDC_NEW_SYMBOL, m_strNewSymbol);
	//}}AFX_DATA_MAP
	
	if (pDX->m_bSaveAndValidate)
	{
		m_strNewSymbol.TrimRight();
		m_strNewSymbol.TrimLeft();

		if (m_strNewSymbol.IsEmpty())
		{
			pDX->PrepareEditCtrl (IDC_NEW_SYMBOL);
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(CAddSymbolDlg, CMktStructureBaseDlg)
	//{{AFX_MSG_MAP(CAddSymbolDlg)
	ON_BN_CLICKED(IDC_ADD_SYMBOL, OnAddSymbol)
	ON_EN_CHANGE(IDC_NEW_SYMBOL, OnChangeNewSymbol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddSymbolDlg message handlers


void CAddSymbolDlg::OnAddSymbol() 
{

	if (! UpdateData (TRUE))
		return;
	
	try
	{
		CDBLayout db;

		db.Connect();

		if (db.IsSymbolExists(m_strNewSymbol) )
		{
			MessageBox ("Symbol already exists");
			return;
		}
	
	}
	catch (_com_error & e)
	{
		MessageBox (e.ErrorInfo()  ? (LPCTSTR)  e.Description() : e.ErrorMessage());
		return;
	}
	
	_bstr_t bstrSymbol = (LPCTSTR) m_strNewSymbol;
	m_stocks.clear();
	m_stocks[ bstrSymbol ] = CContractRequestData();
	m_bAdded = false;
	
	SetWindowPos (NULL, 0, 0, m_sizeFull.cx, m_sizeFull.cy, SWP_NOZORDER | SWP_NOMOVE);

	if (StartDownload (false))
	{
		try
		{
			PP::StockParams stock;
			stock.Exchange = NULL;
			stock.Stock = bstrSymbol;

			m_spStructureInfo->RequestStock(&stock);

			m_edNewSymbol.EnableWindow(FALSE);
			m_btCancel.SetWindowText (_T("Cancel"));
			m_btAdd.EnableWindow(FALSE);
			m_bRequestingData = true;
		}
		catch (_com_error & e)
		{
			ErrorMessage(e);
			Stop();
		}
	}
}

bool CAddSymbolDlg::HandleStock (PP::StockParams *pParams, PP::StockInfo *pResults)
{
	CDBLayout db;

	db.Connect();
	int nID = db.AddNewContract (pParams->Stock, pResults);
	db.Disconnect();

	m_bAdded = true;
	m_bAddedSomething=true;

	CString strFmt;
	strFmt.Format (_T("%s added to the database"), (LPCTSTR) m_strNewSymbol);

	StatusMessage (strFmt);
	StatusMessage (_T("Requesting options"));

	m_stocks[pParams->Stock].m_nID = nID;
	
	if (pResults->StockType == PP::enIndex)
	{
		m_stocks[pParams->Stock].m_bGotOptions = false;
	}

	PP::OptionParams opt;
	opt.Exchange = NULL;
	opt.UnderlyingSymbol = pParams->Stock;

	if (pResults->StockType == PP::enIndex)
		RequestYield();

	m_spStructureInfo->RequestOptions (&opt);

	return true;
}

void CAddSymbolDlg::OnStop()
{
	m_edNewSymbol.EnableWindow (TRUE);
	m_bRequestingData = false;

	if (m_bAdded)
		m_edNewSymbol.SetWindowText(_T(""));
	else
		m_btAdd.EnableWindow(TRUE);


}


void CAddSymbolDlg::OnCancel() 
{
	if (! m_bRequestingData)
		CDialog::OnCancel();
	else
		CMktStructureBaseDlg::OnCancel();
}

void CAddSymbolDlg::OnChangeNewSymbol() 
{
	
	CString strText; 
	m_edNewSymbol.GetWindowText(strText);

	strText.TrimLeft();
	strText.TrimRight();

	m_btAdd.EnableWindow (!strText.IsEmpty());
}

BOOL CAddSymbolDlg::OnInitDialog() 
{
	CMktStructureBaseDlg::OnInitDialog();
	
	CRect rc, rcCancel, rcProgress;
	m_btCancel.GetWindowRect(rcCancel);
	m_pgContract.GetWindowRect(rcProgress);

	GetWindowRect (rc);

	m_sizeFull = rc.Size();

	rc.bottom = rcCancel.bottom + (rcProgress.top - rcCancel.bottom);

	SetWindowPos (NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOMOVE);
	
	CenterWindow();

	if(m_strNewSymbol.GetLength())
	{
		m_btAdd.EnableWindow(TRUE);
		m_edNewSymbol.EnableWindow(FALSE);
		PostMessage(WM_COMMAND,IDC_ADD_SYMBOL,0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
