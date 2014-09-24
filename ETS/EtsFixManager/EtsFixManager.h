// EtsFixManager.h : main header file for the EtsFixManager application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const TCHAR g_cszMainFrameClassName[] = _T("EtsFixManagerMainWindowClassName FC054682-DDF2-414c-8559-46623363E0E7");

// CEtsFixManagerApp:
// See EtsFixManager.cpp for the implementation of this class
//

class CEtsFixManagerApp : public CWinApp
{
public:
	CEtsFixManagerApp();

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
	bool		m_bExitNow;

private:
	static bool _IsFirstInstance();
	void _InitTypeIcons();
	void _LoadTypeIcons(long nResID);
	void _ClearTypeIcons();

public:
	typedef map<long, _variant_t>	CIconByIdMap;
	CIconByIdMap					m_mapTypeIcons;
};

extern CEtsFixManagerApp g_App;