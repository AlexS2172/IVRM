#ifndef __MAIN_WND_H__
#define __MAIN_WND_H__

#include "resource.h"

#include "SystemTray.h"
#include "Settings.h"

class CMainWnd : public CFrameWnd
{
	friend class CServerApp;

public:
	DECLARE_DYNCREATE(CMainWnd)
	CMainWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


// Implementation
protected:
	virtual ~CMainWnd();

	// Generated message map functions
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRtOnline();
	afx_msg void OnRtOffline();
	afx_msg void OnTransportDisabled();
	afx_msg LRESULT OnSettingsUpdate(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateStatistic(WPARAM, LPARAM);
	afx_msg void OnTransportSettings();
	afx_msg void OnClose();
	afx_msg void OnExit();
	afx_msg LRESULT OnTransportServiceError(WPARAM, LPARAM);
	afx_msg LRESULT OnTransportServiceStatus(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	CSystemTray				m_TrayIcon;
	CSettings				m_SettingsSheet;
	bool					m_IsMessBoxActive;
    UINT					m_uIDTimer;

	void					StartTimer()
	{
		m_uIDTimer = SetTimer(1, 5000, NULL);
	}
	void                    StopTimer()
	{
		if(m_uIDTimer)
		{
			KillTimer(m_uIDTimer);
			m_uIDTimer = 0;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__MAIN_WND_H__
