// RtControl.h : main header file for the RTCONTROL application
//

#if !defined(AFX_RTCONTROL_H__6465E655_33D3_41D9_9B52_D70795E2D6C1__INCLUDED_)
#define AFX_RTCONTROL_H__6465E655_33D3_41D9_9B52_D70795E2D6C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define	WM_ICON_NOTIFY				WM_APP + 10
#define WM_ROUTER_SERVICE_ERROR		WM_USER + 10
#define WM_ROUTER_UPDATE_STATUS		WM_USER + 11

#include "HostResolveThread.h"

class CRtControlApp;
typedef IDispEventImpl<1, CRtControlApp, &DIID__IRouterProxyEvents> _IRouterEventsImpl;

/////////////////////////////////////////////////////////////////////////////
// CRtControlApp:
// See RtControl.cpp for the implementation of this class
//
class CRtControlApp : public CWinApp,
	public _IRouterEventsImpl
{
public:
	CRtControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtControlApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRtControlApp)
	afx_msg void OnAppAbout();
	afx_msg void OnRtOpen();
	afx_msg void OnRtStart();
	afx_msg void OnUpdateRtStart(CCmdUI* pCmdUI);
	afx_msg void OnRtStop();
	afx_msg void OnUpdateRtStop(CCmdUI* pCmdUI);
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
		
public:
BEGIN_SINK_MAP (CRtControlApp)
	SINK_ENTRY_INFO(1, DIID__IRouterProxyEvents, DISPID_IRouterProxyEventsOnServiceStatus, OnServiceStatus, &m_OnServiceStatus)
	SINK_ENTRY_INFO(1, DIID__IRouterProxyEvents, DISPID_IRouterProxyEventsOnServiceError, OnServiceError, &m_OnServiceError)
	SINK_ENTRY_INFO(1, DIID__IRouterProxyEvents, DISPID_IRouterProxyEventsOnClientConnected, OnClientConnected, &m_OnClientConnected)
	SINK_ENTRY_INFO(1, DIID__IRouterProxyEvents, DISPID_IRouterProxyEventsOnClientDisconnected, OnClientDisconnected, &m_OnClientDisconnected)
END_SINK_MAP()

protected:
	static _ATL_FUNC_INFO	m_OnServiceStatus;
	static _ATL_FUNC_INFO	m_OnServiceError;
	static _ATL_FUNC_INFO	m_OnClientConnected;
	static _ATL_FUNC_INFO	m_OnClientDisconnected;

	STDMETHOD(OnServiceStatus)(long dwStatus);
	STDMETHOD(OnServiceError)(long nCode, BSTR bsDescription);
	STDMETHOD(OnClientConnected)(BSTR bsHostIP);
	STDMETHOD(OnClientDisconnected)(BSTR bsHostIP);

	IRouterProxyPtr	m_spRT;
	IHostProxyPtr	m_spTP;

public:
	void LoadSettings();
	void SaveSettings();
	void SaveStartType();
	HRESULT AttachToRouter();
	void DetachFromRouter();
	bool GetHostStatistic(BSTR bsHostIP);
	void GetActiveHosts();


	HICON					m_hAppIcon[2];
	HICON					m_hSmallIcon[2];

	CString					m_strStatus[2];
	long					m_nRtStatus;
	BOOL					m_bExitNow;
	long					m_nAutoStart;
	long					m_nStartMin;
	CPoint					m_ptMainWinPos;

	CString					m_strExeName;
	TCHAR					m_szServiceName[256];

	long					m_lUnsentMessNum;
	long					m_lConfMessNum;
	long					m_lRecMessNum;
	long					m_lSendMessNum;
	DATE					m_dtLogonTime;
	
private:
	CHostResolveThread		m_HostResolveThread;
	SC_HANDLE				m_hSCM;
	HANDLE					m_hSingleInstanceMutex;
};

extern CRtControlApp		g_App;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTCONTROL_H__6465E655_33D3_41D9_9B52_D70795E2D6C1__INCLUDED_)
