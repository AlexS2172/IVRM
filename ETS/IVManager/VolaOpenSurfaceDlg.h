#if !defined(AFX_VOLAOPENSURFACEDLG_H__74A5E21A_E180_426B_B3DD_E13A1CDD526F__INCLUDED_)
#define AFX_VOLAOPENSURFACEDLG_H__74A5E21A_E180_426B_B3DD_E13A1CDD526F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolaOpenSurfaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVolaOpenSurfaceDlg dialog

class CVolaOpenSurfaceDlg : public CDialog
{
// Construction
public:
	CVolaOpenSurfaceDlg( CSurfacesVector& vecSurfaces, long& nCurrentSurface, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolaOpenSurfaceDlg)
	enum { IDD = IDD_VM_SURFACE_OPEN_DLG };
	CListBox	m_lbsSurfaces;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolaOpenSurfaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CSurfacesVector& m_vecSurfaces;
	long&			 m_nCurrentSurface;

	// Generated message map functions
	//{{AFX_MSG(CVolaOpenSurfaceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListSurface();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLAOPENSURFACEDLG_H__74A5E21A_E180_426B_B3DD_E13A1CDD526F__INCLUDED_)
