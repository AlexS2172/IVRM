#if !defined(AFX_REMOVESYMBOLDLG_H__E1B42E82_6790_4139_BA70_15AC3C43D441__INCLUDED_)
#define AFX_REMOVESYMBOLDLG_H__E1B42E82_6790_4139_BA70_15AC3C43D441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoveSymbolDlg.h : header file
//
#include "SymbolCombo.h"
/////////////////////////////////////////////////////////////////////////////
// CRemoveSymbolDlg dialog

class CRemoveSymbolDlg : public CDialog
{
// Construction
public:
	CRemoveSymbolDlg(const CSymbolCombo* cbSource,  CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoveSymbolDlg)
	enum { IDD = IDD_REMOVE_SYMBOL };
	CButton	m_OK;
	CComboBox	m_cbSymbols;
	//}}AFX_DATA

	DWORD GetSelectedID(){return m_iSelectedID;}
	void  GetSelectedText(CString& str){str = m_strSelected;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoveSymbolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	const CSymbolCombo* m_pCbSource;
	DWORD				m_iSelectedID;
	CString             m_strSelected;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoveSymbolDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOVESYMBOLDLG_H__E1B42E82_6790_4139_BA70_15AC3C43D441__INCLUDED_)
