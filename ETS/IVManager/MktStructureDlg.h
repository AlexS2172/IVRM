#if !defined(AFX_MKTSTRUCTUREDLG_H__C62EE42A_FC45_4F74_97E2_97E3C6F98103__INCLUDED_)
#define AFX_MKTSTRUCTUREDLG_H__C62EE42A_FC45_4F74_97E2_97E3C6F98103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MktStructureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMktStructureDlg dialog

#include "MktStructureBaseDlg.h"


class CMktStructureDlg :
	public CMktStructureBaseDlg
{
// Construction
public:
	
 	CMktStructureDlg(LPCTSTR szSymbol = NULL, int nID=0, CWnd* pParent = NULL);  // standard constructor
	
	bool IsSymbolUpdated (LPCTSTR szSymbol);

	BOOL ContinueModal();
	

// Dialog Data
	//{{AFX_DATA(CMktStructureDlg)
	enum { IDD = IDD_MKT_STRUCTURE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMktStructureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

protected:

	afx_msg LRESULT OnStartDownload (WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CMktStructureDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MKTSTRUCTUREDLG_H__C62EE42A_FC45_4F74_97E2_97E3C6F98103__INCLUDED_)
