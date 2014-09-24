// RemoveSymbolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "RemoveSymbolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoveSymbolDlg dialog


CRemoveSymbolDlg::CRemoveSymbolDlg(const CSymbolCombo* cbSource, CWnd* pParent /*=NULL*/)
	: CDialog(CRemoveSymbolDlg::IDD, pParent),
	m_pCbSource(cbSource)
{
	//{{AFX_DATA_INIT(CRemoveSymbolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iSelectedID = -1;
}


void CRemoveSymbolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoveSymbolDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_COMBO_SYMBOLS, m_cbSymbols);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoveSymbolDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoveSymbolDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoveSymbolDlg message handlers

BOOL CRemoveSymbolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strData;
	bool bRet = false;

	if(m_pCbSource)
		for (int i=0;i < m_pCbSource->GetCount();i++)
		{
			DWORD dwData = m_pCbSource->GetItemData(i);

			m_pCbSource->GetLBText(i, strData);
			if( dwData>0 && strData.GetLength())
			{
				int idx  = m_cbSymbols.AddString(strData);
				if(idx != CB_ERR  && idx != CB_ERRSPACE)
				{
					m_cbSymbols.SetItemData(idx, dwData);
					bRet = true;
				}
				else
				{
					bRet = false;
					break;
				}
			}
		}

		m_OK.EnableWindow(bRet);
		if(bRet)
		{
			int idx = m_pCbSource->GetCurSel();
			int curNDX = CB_ERR;
			CString str;
			m_pCbSource->GetWindowText(str);
			if(idx!=CB_ERR && m_pCbSource->GetItemData(idx)>0)
			{
				CString strText;
				m_pCbSource->GetLBText(idx, strText);
				if(strText.GetLength())
					curNDX = m_cbSymbols.FindStringExact(-1,(LPCTSTR)strText);

			}
			else
			{
				CString strText;
				m_pCbSource->GetWindowText(strText);
				curNDX = m_cbSymbols.FindStringExact(-1,(LPCTSTR)strText);

			}
			if(CB_ERR == curNDX)
				curNDX = 0;
				
			m_cbSymbols.SetCurSel(curNDX);
			m_cbSymbols.SetFocus();
		}	

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRemoveSymbolDlg::OnOK() 
{
	int curNDX = m_cbSymbols.GetCurSel();
	if(CB_ERR != curNDX)
	{
		m_iSelectedID = m_cbSymbols.GetItemData(curNDX);
		m_cbSymbols.GetLBText(curNDX, m_strSelected);
	}
	CDialog::OnOK();
}
