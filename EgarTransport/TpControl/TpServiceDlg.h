#if !defined(AFX_TPSERVICEDLG_H__A62AA7AE_BEDE_4889_B85B_2C919E6D0FFB__INCLUDED_)
#define AFX_TPSERVICEDLG_H__A62AA7AE_BEDE_4889_B85B_2C919E6D0FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpServiceDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpServiceDlg dialog

class CTpServiceDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTpServiceDlg)

// Construction
public:
	CTpServiceDlg();
	~CTpServiceDlg();

	
	void UpdateTpStatus();

// Dialog Data
	//{{AFX_DATA(CTpServiceDlg)
	enum { IDD = IDD_TRANSPORT_SERVICE };
	CStatic	m_stcStateImage;
	CButton	m_btnStop;
	CButton	m_btnStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTpServiceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTpServiceDlg)
	afx_msg void OnStopButton();
	afx_msg void OnStartButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSERVICEDLG_H__A62AA7AE_BEDE_4889_B85B_2C919E6D0FFB__INCLUDED_)
