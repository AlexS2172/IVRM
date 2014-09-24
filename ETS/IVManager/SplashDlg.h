#if !defined(AFX_SPLASHDLG_H__4EF54163_CB19_401B_8A57_191059BBDE91__INCLUDED_)
#define AFX_SPLASHDLG_H__4EF54163_CB19_401B_8A57_191059BBDE91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashDlg.h : header file
//

#include "PictureEx.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog

class CSplashDlg : public CDialog
{
// Construction
public:
	CSplashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashDlg)
	enum { IDD = IDD_SPLASH };
	CPictureEx	m_picSplash;
	//}}AFX_DATA


	void SetAppearance(HMODULE hAppearance)	{m_hAppearance = hAppearance;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	HMODULE  m_hAppearance;
protected:
	// Generated message map functions
	//{{AFX_MSG(CSplashDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHDLG_H__4EF54163_CB19_401B_8A57_191059BBDE91__INCLUDED_)
