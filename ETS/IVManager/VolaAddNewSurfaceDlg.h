#if !defined(AFX_VOLAADDNEWSURFACEDLG_H__C73244DD_6126_462B_B1D2_A57A8C1EDEA7__INCLUDED_)
#define AFX_VOLAADDNEWSURFACEDLG_H__C73244DD_6126_462B_B1D2_A57A8C1EDEA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolaAddNewSurfaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVolaAddNewSurfaceDlg dialog

class CVolaAddNewSurfaceDlg : public CDialog
{
// Construction
public:
	typedef enum
	{
		enNew,
		enEdit,
		enSaveAs
	}
	Mode;

	CVolaAddNewSurfaceDlg(Mode mode, CSurfacesVector& vecSurfaces, long nCurrentSurface, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolaAddNewSurfaceDlg)
	enum { IDD = IDD_VM_SURFACE_ADD_DLG };
	CString	m_strName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolaAddNewSurfaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Mode			 m_enMode;
	CString			 m_strCaption;
	CSurfacesVector& m_vecSurfaces;
	long			 m_nCurrentSurface;

	// Generated message map functions
	//{{AFX_MSG(CVolaAddNewSurfaceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLAADDNEWSURFACEDLG_H__C73244DD_6126_462B_B1D2_A57A8C1EDEA7__INCLUDED_)
