// HttpUnitTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CHttpUnitTestApp:
// See HttpUnitTest.cpp for the implementation of this class
//

class CHttpUnitTestApp : public CWinApp
{
public:
	CHttpUnitTestApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHttpUnitTestApp theApp;