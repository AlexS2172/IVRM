#if !defined(AFX_VOLANEXTDAYSURFACEDLG_H__062E874E_8E7B_47AF_BAB5_44F0876A667B__INCLUDED_)
#define AFX_VOLANEXTDAYSURFACEDLG_H__062E874E_8E7B_47AF_BAB5_44F0876A667B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolaNextDaySurfaceDlg.h : header file
//

//{{AFX_INCLUDES()
#include "EGButton.h"
#include "..\\EGOcx\\vsflexgrid.h"
//}}AFX_INCLUDES

/////////////////////////////////////////////////////////////////////////////
// CVolaNextDaySurfaceDlg dialog

class CVolaNextDaySurfaceDlg : public CDialog
{
	bool m_bInitialized;

	long m_nMinW, m_nMinH;
	long m_nMargine;
	
// Construction
public:
	CVolaNextDaySurfaceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolaNextDaySurfaceDlg)
	enum { IDD = IDD_VM_NEXT_DAY_SURFACE_DLG };
	CEGButton	m_btnRefresh;
	CEGButton	m_btnClose;
	CVSFlexGrid	m_fgSurfaces;
	//}}AFX_DATA

	void UpdateList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolaNextDaySurfaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CNextDaySurfacesVector m_vecSurfaces;

	void GridFormat();
	void GridFill();

	// Generated message map functions
	//{{AFX_MSG(CVolaNextDaySurfaceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	afx_msg void OnSurfaceDblClick();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSurfaceClick();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void _AlignControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLANEXTDAYSURFACEDLG_H__062E874E_8E7B_47AF_BAB5_44F0876A667B__INCLUDED_)
