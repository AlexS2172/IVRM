// CIBCTradeFeedAdapter.h : main header file for the CIBCTRADEFEEDADAPTER application
//

#if !defined(AFX_CIBCTRADEFEEDADAPTER_H__8CFCC9AB_8111_47C3_93AF_E0DCCE3EBEB4__INCLUDED_)
#define AFX_CIBCTRADEFEEDADAPTER_H__8CFCC9AB_8111_47C3_93AF_E0DCCE3EBEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTFAdapterApp:
// See CIBCTradeFeedAdapter.cpp for the implementation of this class
//

class CTFAdapterApp : public CWinApp
{
private:

// Data members
	HANDLE m_hSingleInstanceMutex;

public:
	CTFAdapterApp();
	~CTFAdapterApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTFAdapterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CTFAdapterApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIBCTRADEFEEDADAPTER_H__8CFCC9AB_8111_47C3_93AF_E0DCCE3EBEB4__INCLUDED_)
