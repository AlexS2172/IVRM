#if !defined(AFX_GROUPSTRUCTDLG_H__59C925D1_C2A1_401D_9AE8_A7D6845B47B9__INCLUDED_)
#define AFX_GROUPSTRUCTDLG_H__59C925D1_C2A1_401D_9AE8_A7D6845B47B9__INCLUDED_

#include "DataDef.h"	// Added by ClassView
#include "EditEx.h"
#include "FlatComboBox.h"
#include "EGButton.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupStructDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupStructDlg dialog


class CGroupStructDlg : public CDialog
{
// Construction
public:
	CGroupStructDlg(CWnd* pParent,
					CGroupsVector& vecGroups,
					CRulesVector& vecRules,
					long nDefGroupIdx, long nCurGroupIdx);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupStructDlg)
	enum { IDD = IDD_VM_GROUP_STRUCT };
	CComboBox	m_cmbRule;
	CEditEx		m_edtName;
	CEGButton	m_btnOK;
	CEGButton	m_btnCancel;
	CEGButton	m_btnRuleEdit;
	CEGButton	m_btnTo;
	CEGButton	m_btnFrom;
	CListBox	m_lstSel;
	CListBox	m_lstAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupStructDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupStructDlg)
	afx_msg void OnButtonFrom();
	afx_msg void OnButtonTo();
	virtual void OnOK();
	afx_msg void OnListSelSelChange();
	afx_msg void OnListAllSelChange();
	afx_msg void OnListAllDblClk();
	afx_msg void OnListSelDblClk();
	virtual BOOL OnInitDialog();
	afx_msg void OnRuleEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void _UpdateButtons();

	CGroupsVector&				m_vecGroups;
	CRulesVector&				m_vecRules;
	long						m_nDefGroupIdx;
	long						m_nCurGroupIdx;

	CNamesMap					m_mapNames;
	CNamesMap					m_mapDefSyms;
	CNamesMap					m_mapCurSyms;
	bool						m_bInitNow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPSTRUCTDLG_H__59C925D1_C2A1_401D_9AE8_A7D6845B47B9__INCLUDED_)
