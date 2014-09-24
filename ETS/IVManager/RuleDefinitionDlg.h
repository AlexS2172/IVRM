#if !defined(AFX_RULEDEFINITIONDLG_H__01FB4798_1157_4640_A548_DB0C9E397939__INCLUDED_)
#define AFX_RULEDEFINITIONDLG_H__01FB4798_1157_4640_A548_DB0C9E397939__INCLUDED_

#include "resource.h"
#include "DataDef.h"	// Added by ClassView
#include "NumericEditEx.h"
#include "FlatComboBox.h"
#include "EGButton.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuleDefinitionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuleDefinitionDlg dialog

class CRuleDefinitionDlg : public CDialog
{
// Construction
public:
	CRuleDefinitionDlg(CWnd* pParent,
					CRulesVector& vecRules,
					long nCurRuleIdx);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRuleDefinitionDlg)
	enum { IDD = IDD_VM_RULE };
	CStatic	m_stcSmileAccelerator;
	CEGButton		m_btnOK;
	CEGButton		m_btnCancel;
	CNumericEditEx	m_edtHiVoly;
	CNumericEditEx	m_edtLoVoly;
	CNumericEditEx	m_edtFactor;
	CNumericEditEx	m_edtPutShift;
	CNumericEditEx	m_edtCallShift;
	CNumericEditEx	m_edtAccelerator;
	CNumericEditEx	m_edtIncrement;
	CEditEx			m_edtName;
	CFlatComboBox	m_cmbType;
	CSliderCtrl		m_sldFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleDefinitionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleDefinitionDlg)
	virtual void OnOK();
	afx_msg void OnTypeSelChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFactorChange();
	virtual BOOL OnInitDialog();
	afx_msg void OnAcceleratorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void _InitControls();

private:
	CRulesVector&		m_vecRules;
	long				m_nCurRuleIdx;

	CNamesMap			m_mapNames;
	int					m_nCurFactorPos;
	bool				m_bInitNow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULEDEFINITIONDLG_H__01FB4798_1157_4640_A548_DB0C9E397939__INCLUDED_)
