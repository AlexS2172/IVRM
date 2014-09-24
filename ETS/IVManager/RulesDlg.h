#if !defined(AFX_RULESDLG_H__89C34153_A7A2_4A09_A98F_A4752BFB5342__INCLUDED_)
#define AFX_RULESDLG_H__89C34153_A7A2_4A09_A98F_A4752BFB5342__INCLUDED_

#include "DataDef.h"	// Added by ClassView
#include "EGButton.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RulesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRulesDlg dialog

class CRulesDlg : public CDialog
{
// Construction
public:
	CRulesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRulesDlg)
	enum { IDD = IDD_VM_RULES };
	CEGButton	m_btnOK;
	CEGButton	m_btnCancel;
	CEGButton	m_btnNew;
	CEGButton	m_btnModify;
	CEGButton	m_btnDelete;
	CListBox	m_lstRules;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRulesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnListRulesSelChange();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonModify();
	afx_msg void OnListRulesDblClk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void LoadRules();

private:
	void _UpdateButtons();
	CString _GetNewName();

private:

	// map for deleted groups
	CIdMap			m_mapDelIDs;
	// rules storage
	CRulesVector	m_vecRules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULESDLG_H__89C34153_A7A2_4A09_A98F_A4752BFB5342__INCLUDED_)
