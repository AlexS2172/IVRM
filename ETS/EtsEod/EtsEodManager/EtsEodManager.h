// EtsEodManager.h : main header file for the EtsEodManager application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const TCHAR g_cszMainFrameClassName[] = _T("EtsEodManagerMainWindowClassName 1F7F0419-F532-4539-8438-893ADDB5150A");

// CEtsEodManagerApp:
// See EtsEodManager.cpp for the implementation of this class
//

class CEtsEodManagerApp : public CWinApp
{
public:
	CEtsEodManagerApp();

	void PersistWindowPlacement(CWnd* pWnd, LPCTSTR lpszWindowName, bool bSave, bool bIsBackground = false);
	_variant_t GetTypeIcon(long nResID);

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	DECLARE_MESSAGE_MAP()

public:
	CString		m_strReportsPath;
	CString		m_strCurrentReportFilePath;
	bool		m_bExitNow;

private:
	bool _InitReportsPath();
	static bool _IsFirstInstance();
	void _InitTypeIcons();
	void _LoadTypeIcons(long nResID);
	void _ClearTypeIcons();

public:
	typedef map<long, _variant_t>	CIconByIdMap;
	CIconByIdMap					m_mapTypeIcons;
};

extern CEtsEodManagerApp g_App;