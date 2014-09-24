#if !defined(AFX_OPTIONSSKEWPAGE_H__E9B229A9_439E_4CA0_ACA5_B5532AF4D2D8__INCLUDED_)
#define AFX_OPTIONSSKEWPAGE_H__E9B229A9_439E_4CA0_ACA5_B5532AF4D2D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsSkewPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsSkewPage dialog

class COptionsSkewPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsSkewPage)

// Construction
public:
	COptionsSkewPage();
	~COptionsSkewPage();

// Dialog Data
	//{{AFX_DATA(COptionsSkewPage)
	enum { IDD = IDD_OPT_SKEW };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsSkewPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsSkewPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSKEWPAGE_H__E9B229A9_439E_4CA0_ACA5_B5532AF4D2D8__INCLUDED_)
