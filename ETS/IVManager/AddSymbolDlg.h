#if !defined(AFX_ADDSYMBOLDLG_H__31853731_71A7_41A7_9876_1F0F26C3AE81__INCLUDED_)
#define AFX_ADDSYMBOLDLG_H__31853731_71A7_41A7_9876_1F0F26C3AE81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddSymbolDlg.h : header file
//

#include "MktStructureBaseDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAddSymbolDlg dialog

class CAddSymbolDlg : public CMktStructureBaseDlg
{
// Construction
public:
	CAddSymbolDlg(LPCTSTR szSymbol = NULL, CWnd* pParent = NULL);   // standard constructor
	bool IsAddedSomething() {return (m_bAdded);}
// Dialog Data
	//{{AFX_DATA(CAddSymbolDlg)
	enum { IDD = IDD_ADD_SYMBOL };
	CButton	m_btAdd;
	CEdit	m_edNewSymbol;
	CString	m_strNewSymbol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSymbolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool HandleStock (PP::StockParams *pParams, PP::StockInfo *pResults);
	void OnStop();

	bool m_bAdded;
	bool m_bAddedSomething;
	bool m_bRequestingData;
	
	CSize m_sizeFull;

	// Generated message map functions
	//{{AFX_MSG(CAddSymbolDlg)
	afx_msg void OnAddSymbol();
	virtual void OnCancel();
	afx_msg void OnChangeNewSymbol();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSYMBOLDLG_H__31853731_71A7_41A7_9876_1F0F26C3AE81__INCLUDED_)
