#if !defined(AFX_VOLAEDITORDLG_H__AFF787E3_5308_4D5B_973A_E2A50672DA1A__INCLUDED_)
#define AFX_VOLAEDITORDLG_H__AFF787E3_5308_4D5B_973A_E2A50672DA1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolaEditorDlg.h : header file
//

#include "EGButton.h"
#include "NumericEditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CVolaEditorDlg dialog

class CVolaEditorDlg : public CDialog
{
// Construction
public:
	CVolaEditorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolaEditorDlg)
	enum { IDD = IDD_VOLA_EDIT_DIALOG };
	CNumericEditEx	m_edNewVola;
	CNumericEditEx	m_edNewStrike;
	CNumericEditEx	m_edNewPrice;
	CNumericEditEx	m_edCurVola;
	CNumericEditEx	m_edCurStrike;
	CNumericEditEx	m_edCurPrice;
	CButton	m_btOK;
	CButton	m_btCancel;
	//}}AFX_DATA

	void SetCurVola( double dVola );
	void SetNewVola( double dVola );
	void SetCurStrike( double dStrike );
	void SetNewStrike( double dStrike );
	
	double	m_dCurPrice;
	double	m_dCurVola;
	double	m_dNewPrice;
	double	m_dNewVola;
	double	m_dCurStrike;
	double	m_dNewStrike;

	DATE	m_dtExpiration;
	double	m_dPrice;	
	bool	m_bIsCall;

	long    m_nTrackChangesSemaphor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolaEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void CalcCurPrice();
	void CalcNewPrice();
	void CalcNewVola();

	// Generated message map functions
	//{{AFX_MSG(CVolaEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeNewVola();
	virtual void OnOK();
	afx_msg void OnChangeNewPrice();	  
	//}}AFX_MSG
	void OnCommitNewVola();
	void OnDiscardNewVola();
	void OnCommitNewPrice();
	void OnDiscardNewPrice();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLAEDITORDLG_H__AFF787E3_5308_4D5B_973A_E2A50672DA1A__INCLUDED_)
