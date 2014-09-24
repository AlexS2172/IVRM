#if !defined(AFX_OPTIONSCONNECTIONPAGE_H__96A1BC6C_CC03_4AA3_815D_BFD7378F3B95__INCLUDED_)
#define AFX_OPTIONSCONNECTIONPAGE_H__96A1BC6C_CC03_4AA3_815D_BFD7378F3B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsConnectionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage dialog

class COptionsConnectionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsConnectionPage)

// Construction
public:
	COptionsConnectionPage();
	~COptionsConnectionPage();

// Dialog Data
	//{{AFX_DATA(COptionsConnectionPage)
	enum { IDD = IDD_OPT_CONNECTION };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsConnectionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsConnectionPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCONNECTIONPAGE_H__96A1BC6C_CC03_4AA3_815D_BFD7378F3B95__INCLUDED_)
