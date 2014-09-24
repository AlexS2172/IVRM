#if !defined(AFX_SYMBOLGROUPSDLG_H__1342EDBD_C595_40C3_A280_B15121FA444C__INCLUDED_)
#define AFX_SYMBOLGROUPSDLG_H__1342EDBD_C595_40C3_A280_B15121FA444C__INCLUDED_

#include "DataDef.h"	// Added by ClassView
#include "EGButton.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolGroupsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//
class CSymbolGroupsDlg : public CDialog
{
// Construction
public:
	CSymbolGroupsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSymbolGroupsDlg)
	enum { IDD = IDD_VM_SYMBOL_GROUPS };
	CEGButton	m_btnOK;
	CEGButton	m_btnCancel;
	CEGButton	m_btnNew;
	CEGButton	m_btnModify;
	CEGButton	m_btnDelete;
	CListBox	m_lstGroups;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolGroupsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSymbolGroupsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnListGroupsSelChange();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonModify();
	afx_msg void OnListGroupsDblClk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void LoadGroups();

private:
	void _UpdateButtons();
	CString _GetNewName();

private:
	
	// map for deleted groups
	CIdMap			m_mapDelIDs;
	// groups storage
	CGroupsVector	m_vecGroups;

	// rules storage
	CRulesVector	m_vecRules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLGROUPSDLG_H__1342EDBD_C595_40C3_A280_B15121FA444C__INCLUDED_)
