// TradesUploader.h : main header file for the TRADESUPLOADER application
//

#if !defined(AFX_TRADESUPLOADER_H__A5AFA44E_46A7_4CB8_A7A5_97E87932CCF4__INCLUDED_)
#define AFX_TRADESUPLOADER_H__A5AFA44E_46A7_4CB8_A7A5_97E87932CCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderApp:
// See TradesUploader.cpp for the implementation of this class
//

class CTradesUploaderApp : public CWinApp
{
public:
	CTradesUploaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradesUploaderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	void PersistWindowPlacement(CWnd*  pWnd, LPCTSTR RelativePath, BOOL bDirection);

	//{{AFX_MSG(CTradesUploaderApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADESUPLOADER_H__A5AFA44E_46A7_4CB8_A7A5_97E87932CCF4__INCLUDED_)
