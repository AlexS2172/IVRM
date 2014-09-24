// SageApp.h : main header file for the SAGE application
//

#if !defined(__SAGEAPP_H__)
#define __SAGEAPP_H__

/////////////////////////////////////////////////////////////////////////////
// CSageApp:
// See SageApp.cpp for the implementation of this class
//

#define APP_KEY     _T("Egar\\ETS")
#include "resource_gui.h"		// main symbols

class CSageApp : public CWinApp
{
public:
	CSageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
    void PersistWindowPlacement(CWnd*  pWnd, LPCTSTR ReleaitivePath, BOOL bDirection);

	//{{AFX_MSG(CSageApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    DECLARE_APPREG()

protected:
	HANDLE		m_hSingleInstanceMutex;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SAGEAPP_H__)
